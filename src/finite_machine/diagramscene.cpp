#include "diagramscene.h"
#include <iostream>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include <limits>
#include "finite_machine/arrow.h"

DiagramScene::DiagramScene(FiniteAutomata* _FA, QWidget *parent = 0) : QGraphicsScene(parent)
{
    this->actMode = AddNodeMode;
    clicked = false;
    actLine = 0;
    FA = _FA;
    startingState = NULL;
    connect(this, SIGNAL(selectionChanged()), this, SLOT(changeSelected()));

    //for future random generate
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}


void DiagramScene::setMode(DiagramScene::Mode mode)
{
    this->actMode = mode;

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
             newNode = new StateNode(this, FA);
             this->addItem(newNode);
             FA->addState(newNode->getName());
             newNode->setPos(mouseEvent->scenePos());
             FA->coordinates[newNode->getName()] = mouseEvent->scenePos().toPoint();
             if(newNode->getName() == "0" && startingState == NULL)
                 newNode->setStartinState();

             emit FA_changed(FA);
             break;
				case AddArrowMode:
             //TODO predelat caru na sipku
             actLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                         mouseEvent->scenePos()));
             actLine->setPen(QPen(Qt::black, 2));
             addItem(actLine);
             //emit FA_changed(FA);
             break;
        case MoveNodeMode:
             break;
        case DeleteNodeMode:
            break;
     }

//         QBrush redBrush(Qt::red);
//         QPen   bluePen(Qt::blue);
//         QPointF pos = mouseEvent->scenePos();
//         QGraphicsEllipseItem* elipse = this->addEllipse(pos.x()-25.0,pos.y()-25.0,50,50,bluePen,redBrush);
//         elipse->setFlag(QGraphicsItem::ItemIsMovable, true);
//         elipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
//         elipse->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
     QGraphicsScene::mousePressEvent(mouseEvent);

     //cant unselect node because selection set also mouse release event :(
//     // Unselect node
//     if(newNode != 0)
//         newNode->setSelected(false);
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

 void DiagramScene::AddArrow(StateNode *startItem, StateNode *endItem){
	 SymbolsInputDialog inputDialog("");
	 QStringList symbols;
	 if(QDialog::Accepted == inputDialog.exec())
	 {
         symbols = inputDialog.symbols;
         Arrow* arrow = new Arrow(startItem, endItem, FA, symbols,0,this);
         QStringList new_symbols;
         foreach(QString symbol,symbols)
         {
            if(FA->addRule(ComputationalRules(startItem->getName(),endItem->getName(),symbol)))
            {
                if(symbol != EPSILON)
                {
                    FA->addSymbol(symbol);
                    new_symbols.append(symbol);
                }
                emit FA_changed(FA);
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

 //pokud jsme v modu ruseni zulu pak po oznaceni uzlu ho zrus
 void DiagramScene::changeSelected()
 {
     if(actMode == DeleteNodeMode && !this->selectedItems().isEmpty())
     {
         std::cout<<"call DeleteSelected()"<<std::endl;
         deleteSelected();
     }
 }

void DiagramScene::addNodes(QSet<QString> nodes, QMap<QString,QPoint> coordinates)
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

void DiagramScene::removeNodes(QSet<QString> nodes)
{
    foreach(QString nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        removeItem(node);
        if(node == startingState)
        {
            startingState = NULL;
            FA->startState = "";
        }
        foreach(Arrow* arrowToRemove,node->arrows)
            removeItem(arrowToRemove);
        delete node;
    }
    emit FA_changed(FA);
}

void DiagramScene::deleteSelected()
{
    QList<QGraphicsItem*> items = this->selectedItems();


    //first delete edges
    foreach(QGraphicsItem* item,items)
    {
        Arrow* arrow = dynamic_cast<Arrow*>(item);
        if (arrow)
        {
            removeItem(arrow);
            items.removeOne(arrow);
            foreach(QString symbol,arrow->m_symbols)
                FA->removeRule(ComputationalRules(arrow->startItem()->getName(),arrow->endItem()->getName(),symbol));
            delete arrow;
        }
    }



    //then delete nodes
    foreach(QGraphicsItem* item,items)
    {
        StateNode* node = dynamic_cast<StateNode*>(item);
        if (node)
        {
            FA->removeState(node->getName());
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
    emit FA_changed(FA);

}

void DiagramScene::setStartNode(QString nodeName)
{
    StateNode* node = getNodeByName(nodeName);
    startingState = node;
    if(node != NULL)
        node->setStartinState();
}

void DiagramScene::addEndingNodes(QSet<QString> nodes)
{
    QString nodeName;
    foreach(nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        if(node != NULL)
            node->setEndingState(true);
    }
}

void DiagramScene::removeEndingNodes(QSet<QString> nodes)
{
    QString nodeName;
    foreach(nodeName,nodes)
    {
        StateNode* node = getNodeByName(nodeName);
        if(node != NULL)
            node->setEndingState(false);
    }
}

void DiagramScene::addEdges(QSet<ComputationalRules> rules)
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
            Arrow *newArrow = new Arrow(from, to, FA, symbolList,0,this);
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
        FA->addRule(rule);
    }
    emit FA_changed(FA);
}

void DiagramScene::removeEdges(QSet<ComputationalRules> rules)
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

void DiagramScene::selectAll()
{
    foreach(QGraphicsItem* item,this->items())
    {
        item->setSelected(true);
    }
}

QPoint DiagramScene::randGeneratePos()
{
    QGraphicsView* view = dynamic_cast<QGraphicsView*> (this->parent());


    QRect viewport_rect(0, 0, view->viewport()->width(), view->viewport()->height());
    QRectF visible_scene_rect = view->mapToScene(viewport_rect).boundingRect();

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
    StateNode* newNode = new StateNode(this, this->FA, node_name);
    this->addItem(newNode);
    if(point != QPoint(-1,-1))
    {
        newNode->setPos(point);
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
                FA->coordinates[node_name] = new_point;
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
        FA->coordinates[node_name] = best_point;
    }
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
            //FA->removeState(node->)
        }
        else
            qWarning("Warning: recoverable errors in diagramscene.cpp, which could not never happen.");
		}
}

void DiagramScene::emit_FA_changed(FiniteAutomata *FA)
{
    emit FA_changed(FA);
}

void DiagramScene::setFA(FiniteAutomata* _FA)
{
    clean();
    clear();
    clearFocus();
    clearSelection();
    this->FA = _FA;
    addNodes(FA->states, FA->coordinates);
    setStartNode(FA->startState);
    addEdges(FA->rules);
    addEndingNodes(FA->finalStates);
    emit FA_changed(FA);
}


