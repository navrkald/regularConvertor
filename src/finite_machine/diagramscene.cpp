#include "diagramscene.h"
#include <iostream>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include <limits>
#include "finite_machine/arrow.h"

DiagramScene::DiagramScene(FiniteAutomata* _FA, QWidget *parent = 0) : QGraphicsScene(parent)
{
    this->actMode = MoveNodeMode;
    clicked = false;
    actLine = 0;
    m_fa = _FA;
    startingState = NULL;
    connect(this, SIGNAL(selectionChanged()), this, SLOT(ChangeSelected()));

    //for future random generate
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}


void DiagramScene::SetMode(DiagramScene::Mode mode)
{
    this->actMode = mode;
}

StateNode* DiagramScene::CreateStateNode(QString nodeName){
    if(nodeName.isEmpty()){
        nodeName = CreateNodeUniqueName();
    }

    AddNewState(nodeName);

    return new StateNode(this, nodeName);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
     if (mouseEvent->button() != Qt::LeftButton)
         return;

     clicked = true;
     StateNode* newNode = 0;
     switch(this->actMode)
     {
        case AddNodeMode:
        {
             newNode = CreateStateNode();
             this->addItem(newNode);
             QPointF qNodePos = mouseEvent->scenePos();
             QString nodeName = newNode->getName();
             newNode->setPos(qNodePos);
             AddNewState(nodeName, qNodePos.toPoint());
             if(nodeName == "0" && startingState == NULL)
                 newNode->setStartingState();
             break;
        }
        case AddArrowMode:
             //TODO predelat caru na sipku
             actLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                         mouseEvent->scenePos()));
             actLine->setPen(QPen(Qt::black, 2));
             addItem(actLine);
             break;
        case MoveNodeMode:
             break;
        case DeleteNodeMode:
            break;
     }

     QGraphicsScene::mousePressEvent(mouseEvent);
 }

 void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
		 if (actMode == AddArrowMode && actLine != 0)
     {
         actLine->setLine(actLine->line().p1().x(),actLine->line().p1().y(), mouseEvent->scenePos().x(),mouseEvent->scenePos().y());
     }
     else //if (actMode == MoveNode)
         QGraphicsScene::mouseMoveEvent(mouseEvent);
}

 void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
    clicked = false;

    if (actLine != 0 && actMode == AddArrowMode)
    {
        QList<QGraphicsItem *> startItems = items(actLine->line().p1());
        if (startItems.count() && startItems.first() == actLine)
            startItems.removeFirst();

        QList<QGraphicsItem *> endItems = items(actLine->line().p2());
        if (endItems.count() && endItems.first() == actLine)
         endItems.removeFirst();

        removeItem(actLine);
        delete actLine;
        actLine = 0;

        if (startItems.count() > 0 && endItems.count() > 0
        && 0 != qgraphicsitem_cast<StateNode *>(startItems.first())
        && 0 != qgraphicsitem_cast<StateNode *>(endItems.first()))
        {
        StateNode *startItem = qgraphicsitem_cast<StateNode *>(startItems.first());
        StateNode *endItem = qgraphicsitem_cast<StateNode *>(endItems.first());
        Arrow *arrow;
        if((arrow = startItem->hasArrowTo(endItem)) != 0)
        {
            arrow->EditArrow();
            return;
        }
        AddArrow(startItem, endItem);
    }
}
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
 }

 //pokud jsme v modu ruseni zulu pak po oznaceni uzlu ho zrus
 void DiagramScene::ChangeSelected()
 {
     if(actMode == DeleteNodeMode && !this->selectedItems().isEmpty())
     {
         std::cout<<"call DeleteSelected()"<<std::endl;
         DeleteSelected();
     }
 }

void DiagramScene::AddNodes(QSet<QString> nodes, QMap<QString,QPoint> coordinates)
{
    QString node;
    foreach(node,nodes)
    {
        if(coordinates.contains(node))
            addNode(node,coordinates[node]);
        else
            addNode(node,QPoint(-1,-1));
    }
}

void DiagramScene::RemoveNodes(QSet<QString> nodes)
{
    foreach(QString nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        removeItem(node);
        if(node == startingState)
        {
            startingState = NULL;
            SetStartState("", false);
        }
        foreach(Arrow* arrowToRemove,node->arrows)
            removeItem(arrowToRemove);
        delete node;
    }
    EmitFiniteAutomataChanged();
}

void DiagramScene::DeleteSelected()
{
    QList<QGraphicsItem*> items = this->selectedItems();


    //first delete arrows
    foreach(QGraphicsItem* item,items)
    {
        CheckArrowTypeAndDelete(item, items);
    }

    //then delete nodes
    foreach(QGraphicsItem* item,items)
    {
        StateNode* node = dynamic_cast<StateNode*>(item);
        if (node)
        {
            RemoveStateFromFiniteAutomata(node->getName());
            removeItem(node);
            if(node == startingState)
            {
                this->startingState = NULL;
            }
            foreach(Arrow* arrowToRemove,node->arrows)
                removeItem(arrowToRemove);
            delete node;
        }
        else
            qWarning("Warning: recoverable errors in diagramscene.cpp, which could not never happen.");
    }
        EmitFiniteAutomataChanged();

}

void DiagramScene::SetStartNode(QString nodeName)
{
    StateNode* node = getNodeByName(nodeName);
    startingState = node;
    if(node != NULL)
        node->setStartingState();
}

void DiagramScene::AddEndingNodes(QSet<QString> nodes)
{
    QString nodeName;
    foreach(nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        if(node != NULL)
            node->setEndingState(true);
    }
}

void DiagramScene::RemoveEndingNodes(QSet<QString> nodes)
{
    QString nodeName;
    foreach(nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        if(node != NULL)
            node->setEndingState(false);
    }
}



void DiagramScene::RemoveEdges(QSet<ComputationalRules> rules)
{
    ComputationalRules rule;
    foreach(rule,rules)
    {
        StateNode* from = getNodeByName(rule.from);
        StateNode* to = getNodeByName(rule.to);
        QString symbol = rule.symbol;
        Arrow* arrow = getArrow(from,to);
        if(arrow->removeSymbol(symbol))
        {//remove arrow
            delete arrow;
        }
    }
}

void DiagramScene::SelectAll()
{
    foreach(QGraphicsItem* item,this->items())
    {
        item->setSelected(true);
    }
}

QRectF DiagramScene::GetVisibleSceneRect(){
    QGraphicsView* view = dynamic_cast<QGraphicsView*> (this->parent());
    QRect viewport_rect(0, 0, view->viewport()->width(), view->viewport()->height());
    return view->mapToScene(viewport_rect).boundingRect();
}

QPoint DiagramScene::randGeneratePos()
{
    QRectF visible_scene_rect = GetVisibleSceneRect();

    int x_low = visible_scene_rect.x() + NODE_RADIUS;
    int y_low = visible_scene_rect.y() + NODE_RADIUS;

    int x_high = visible_scene_rect.x() + visible_scene_rect.width() - NODE_RADIUS;
    int y_higt = visible_scene_rect.y() + visible_scene_rect.height() - NODE_RADIUS;
    QPoint point = QPoint(qrand() % ((x_high + 1) - x_low) + x_low, qrand() % ((y_higt + 1) - y_low) + y_low);
    return point;

}

//add node to qgraphic scene
void DiagramScene::addNode(QString node_name, QPoint point)
{
    StateNode* newNode = CreateStateNode(node_name);
    this->addItem(newNode);
    if(point != QPoint(-1,-1))
    {
        newNode->setPos(point);

    }
    else if(IsFirstNode()){
        QRectF visibleSceneRect = GetVisibleSceneRect();
        QPointF inMiddleOfVisibleScene = visibleSceneRect.center();
        newNode->setPos(inMiddleOfVisibleScene);
        SetNodeCoordinates(node_name, inMiddleOfVisibleScene.toPoint());
    }
    else
    {   // Generate random pos of node
        int least_num_colide_items = std::numeric_limits<int>::max();
        QPoint best_point;
        for(int i=0;i<NUM_OF_TRYES;i++)
        {
            QPoint new_point = randGeneratePos();
            newNode->setPos(new_point);
            QList<QGraphicsItem *> colide_items = newNode->collidingItems();
            if(0 == colide_items.count())
            {
                SetNodeCoordinates(node_name, new_point);
                return;
            }
            else
            {
                if(colide_items.count()<least_num_colide_items)
                {
                    least_num_colide_items = colide_items.count();
                    best_point = new_point;
                }
            }
        }
        newNode->setPos(best_point);
        SetNodeCoordinates(node_name, best_point);
    }
}

bool DiagramScene::IsFirstNode(){
    QGraphicsItem* item;
    int numOfNodes = 0;
    foreach(item,this->items())
    {
        StateNode* node = dynamic_cast<StateNode*>(item);
        if(node)
        {
            numOfNodes++;
        }
    }
    return numOfNodes == 1;
}

StateNode *DiagramScene::getNodeByName(QString nodeName)
{
    QGraphicsItem* item;
    foreach(item,this->items())
    {
        StateNode* node = dynamic_cast<StateNode*>(item);
        if(node && node->getName() == nodeName)
        {
            return node;
        }
    }
    return NULL;
}


 DiagramScene::Mode DiagramScene::getMode()
 {
     return actMode;
 }

Arrow* DiagramScene::getArrow(StateNode *from, StateNode* to)
{
    QGraphicsItem* item;
    foreach(item,this->items())
    {
         Arrow* arrow = dynamic_cast<Arrow*>(item);
         if(arrow && arrow->m_myStartItem == from && arrow->m_myEndItem == to)
         {
             return arrow;
         }
    }
    return NULL;
}

void DiagramScene::clean()
{
    actLine=0;
    startingState=0;
    QList<QGraphicsItem*> items = this->items();

    //first delete edges
    foreach(QGraphicsItem* item,items)
    {
        Arrow* arrow = dynamic_cast<Arrow*>(item);
        if (arrow)
        {
            removeItem(arrow);
            items.removeOne(arrow);
            delete arrow;
        }
    }

    //then delete nodes
    foreach(QGraphicsItem* item,items)
    {
        StateNode* node = dynamic_cast<StateNode*>(item);
        if (node)
        {
            removeItem(node);
            foreach(Arrow* arrowToRemove,node->arrows)
            removeItem(arrowToRemove);
            delete node;
        }
        else
            qWarning("Warning: recoverable errors in diagramscene.cpp, which could not never happen.");
    }
}

/************************************************************************
             Functions working with m_fa
************************************************************************/

void DiagramScene::AddEdges(QSet<ComputationalRules> rules)
{
    foreach(ComputationalRules rule,rules)
    {
        StateNode* from = getNodeByName(rule.from);
        StateNode* to = getNodeByName(rule.to);
        QString symbol = rule.symbol;
        Arrow* arrow = getArrow(from,to);
        if(arrow == NULL)
        {
            QStringList symbolList;
            symbolList.append(symbol);
            Arrow *newArrow = new Arrow(from, to, m_fa, symbolList,0,this);
            from->addArrow(newArrow);
            to->addArrow(newArrow);
            newArrow->setZValue(-1000.0);   //posun na pozadi
            addItem(newArrow);
            newArrow->updatePosition();
        }
        else
        {
            arrow->addSymbol(symbol);
            arrow->updatePosition();
        }
        m_fa->addRule(rule);
    }
	EmitFiniteAutomataChanged();

}

QString DiagramScene::CreateNodeUniqueName(){
    return m_fa->createUniqueName();
}

void DiagramScene::AddNewState(QString nodeName, QPoint position)
{
    if(nodeName.isEmpty()){
        nodeName = CreateNodeUniqueName();
    }
    m_fa->addState(nodeName);
    if(!position.isNull())
        SetNodeCoordinates(nodeName, position);
    EmitFiniteAutomataChanged();
}

void DiagramScene::AddArrow(StateNode *startItem, StateNode *endItem){
    SymbolsInputDialog inputDialog("");
    QStringList symbols;
    if(QDialog::Accepted == inputDialog.exec())
    {
        symbols = inputDialog.symbols;
        Arrow* arrow = new Arrow(startItem, endItem, m_fa, symbols,0,this);
        QStringList new_symbols;
        foreach(QString symbol,symbols)
        {
           if(m_fa->addRule(ComputationalRules(startItem->getName(),endItem->getName(),symbol)))
           {
               if(symbol != EPSILON)
               {
                   m_fa->addSymbol(symbol);
                   new_symbols.append(symbol);
               }
               emit FA_changed(m_fa);
           }
           else
           {
               emit sendStatusBarMessage(tr("WARNING: Your set existing edge."));
           }
        }
        startItem->addArrow(arrow);
        endItem->addArrow(arrow);
        arrow->setZValue(-1000.0);   //posun na pozadi
        addItem(arrow);
        arrow->updatePosition();
    }
}

void DiagramScene::CheckArrowTypeAndDelete(QGraphicsItem *item, QList<QGraphicsItem*>& items)
{
    Arrow* arrow = dynamic_cast<Arrow*>(item);
    if (arrow)
    {
        removeItem(arrow);
        items.removeOne(arrow);
        foreach(QString symbol,arrow->m_symbols)
            m_fa->removeRule(ComputationalRules(arrow->startItem()->getName(),arrow->endItem()->getName(),symbol));
        delete arrow;
    }
}

void DiagramScene::EmitFiniteAutomataChanged()
{
    emit FA_changed(m_fa);
}

void DiagramScene::RemoveStateFromFiniteAutomata(QString state)
{
    m_fa->removeState(state);
}

void DiagramScene::SetFa(FiniteAutomata* _FA)
{
    clean();
    clear();
    clearFocus();
    clearSelection();
    m_fa = _FA;
    AddNodes(m_fa->m_states, m_fa->m_coordinates);
    SetStartNode(m_fa->m_startState);
    AddEdges(m_fa->m_rules);
    AddEndingNodes(m_fa->m_finalStates);
    EmitFiniteAutomataChanged();
}

void DiagramScene::SetStartState(QString nodeName, bool emitFaChanged){
    m_fa->SetStartState(nodeName);
    if(emitFaChanged)
        EmitFiniteAutomataChanged();
}

void DiagramScene::SetEndingState(QString nodeName, bool isEnding){
    //setup FA
    if(isEnding)
        m_fa->addFinalState(nodeName);
    else
        m_fa->removeFinalState(nodeName);

    EmitFiniteAutomataChanged();
}

void DiagramScene::SetNodeCoordinates(QString nodeName, QPoint position){
    m_fa->m_coordinates[nodeName] = position;
}

bool DiagramScene::CanSetNodeName(QString newName){
    return m_fa->isStateUnique(newName);
}

void DiagramScene::RenameNode(QString oldName, QString newName){
    if(CanSetNodeName(newName)){
        m_fa->RenameState(oldName, newName);
        EmitFiniteAutomataChanged();
    }
}
