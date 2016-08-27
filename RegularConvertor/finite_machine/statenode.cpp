#include "statenode.h"
#include <iostream>
#include <QGraphicsItem>
#include <QWidget>

unsigned int StateNode::ID_counter = 1;

int StateNode::type() const
{
        // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}

Arrow* StateNode::hasArrowTo(StateNode *node_to)
{
    QSet<Arrow*> same_arrows = arrows.toSet().intersect(node_to->arrows.toSet());
    foreach(Arrow* arrow,same_arrows)
    {
        if(arrow->startItem() == this && arrow->endItem() == node_to)
            return arrow;
    }
    return 0;
}

StateNode::StateNode(DiagramScene* scene, FiniteAutomata* _FA)
{

    FA = _FA;
    myscene = scene;
    node_name = FA->createUniqueName();
    firstInit();

    connect(this,SIGNAL(sendStatusBarMessage(QString)),scene,SIGNAL(sendStatusBarMessage(QString)));
    connect(this,SIGNAL(FA_changed(FiniteAutomata*)),scene,SIGNAL(FA_changed(FiniteAutomata*)));
    connect(this,SIGNAL(xChanged()),this,SLOT(positionChanged()));
    connect(this,SIGNAL(yChanged()),this,SLOT(positionChanged()));


    FA->addState(node_name);
    emit FA_changed(FA);
}

StateNode::StateNode(DiagramScene *scene, FiniteAutomata *_FA, QString uniqueName)
{
    FA = _FA;
    myscene = scene;
    node_name = uniqueName;
    firstInit();

    connect(this,SIGNAL(sendStatusBarMessage(QString)),scene,SIGNAL(sendStatusBarMessage(QString)));
    connect(this,SIGNAL(FA_changed(FiniteAutomata*)),scene,SIGNAL(FA_changed(FiniteAutomata*)));
    connect(this,SIGNAL(xChanged()),this,SLOT(positionChanged()));
    connect(this,SIGNAL(yChanged()),this,SLOT(positionChanged()));

    FA->addState(node_name);
    emit FA_changed(FA);


}

void StateNode::firstInit()
{
        this->endingState = false;
        radius = NODE_RADIUS;
        nodeBrush = new QBrush(Qt::yellow);

        nodePen.setColor(Qt::black);
        nodePen.setWidth(NODE_PEN_WIDTH);
        selected = false;
        pressed = false;
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

StateNode::~StateNode()
{
    removeArrows();
}

QRectF StateNode::boundingRect() const
{
    if(myscene->startingState == this)
        return elipseBoundingRect().adjusted(-START_STATE_ARROW_LENTH,0,+START_STATE_ARROW_LENTH,0);
    else
        return elipseBoundingRect();
}



void StateNode::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void StateNode::setStartinState()
{
    if(myscene->startingState != NULL)
    {
        myscene->startingState->prepareGeometryChange();
    }
    myscene->startingState = this;
    FA->SetStartState(getName());
    emit FA_changed(FA);
    update();
}

void StateNode::setEndingState(bool _endingState)
{
    //setup FA
    if(_endingState)
        FA->addFinalState(getName());
    else
        FA->removeFinalState(getName());

    endingState = _endingState;
    update();
    emit FA_changed(FA);
}

void StateNode::positionChanged()
{
    FA->coordinates[node_name] = this->pos().toPoint();
    qDebug() << node_name << ":" << this->pos().toPoint();
}

void StateNode::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        //arrow->startItem()->removeArrow(arrow);
        //arrow->endItem()->removeArrow(arrow);
        delete arrow;
    }
}

void StateNode::addArrow(Arrow *arrow)
{
    if(!arrows.contains(arrow))
        arrows.append(arrow);
}

QVariant StateNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    if (change == QGraphicsItem::ItemSelectedChange)
    {
        bool bool_val = value.toBool();
        if (bool_val == true)
        {
            selected = true;
            update();
        }
        else
        {
            selected = false;
            update();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

double StateNode::getRadius()
{
   return radius;
}

void StateNode::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setBrush(*nodeBrush);
    if(selected)
    {
        QPen selectedPen(Qt::DashLine);
        painter->setPen(selectedPen);        
    }
    else
    {
        painter->setPen(nodePen);
    }
    painter->drawEllipse(elipseBoundingRect());
    painter->drawText(elipseBoundingRect(), Qt::AlignCenter, node_name);
    if(this->endingState)
    {
        QRectF smalerRect = elipseBoundingRect();
        smalerRect.setWidth(smalerRect.width()-ENDING_STATE_PAADING);
        smalerRect.setHeight(smalerRect.height()-ENDING_STATE_PAADING);
        smalerRect.moveCenter(elipseBoundingRect().center());
        painter->setBrush(Qt::NoBrush);
        painter->setPen(nodePen);
        painter->drawEllipse(smalerRect);
    }
    if(myscene->startingState == this)
    {
        qreal centerLeft_y = elipseBoundingRect().bottom() - elipseBoundingRect().height()/2.0;
        qreal centerLeft_x = elipseBoundingRect().left();
        QPen startStateArrowPen;
        startStateArrowPen.setColor(Qt::black);
        startStateArrowPen.setWidth(START_STATE_ARROW_PEN_WIDTH);
        painter->setPen(startStateArrowPen);
        painter->drawLine(centerLeft_x,centerLeft_y,centerLeft_x-START_STATE_ARROW_LENTH,centerLeft_y);
        painter->drawLine(centerLeft_x,centerLeft_y,centerLeft_x-START_STATE_ARROW_LENTH/2,centerLeft_y-elipseBoundingRect().height()/4.0);
        painter->drawLine(centerLeft_x,centerLeft_y,centerLeft_x-START_STATE_ARROW_LENTH/2,centerLeft_y+elipseBoundingRect().height()/4.0);
    }
}


void StateNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //TODO
    if(myscene->actMode == DiagramScene::MoveNodeMode)
    {
        //TODO po kliknuti na uzel se automaticky presune na popredi + snizi se vsechny hodnoty na minimun (mozna snizeni hodnot v samostatnem thredu...)
        ;
    }
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void StateNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void StateNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(myscene->actMode == DiagramScene::MoveNodeMode)
    {
        QString text = QInputDialog::getText(event->widget(),tr("New node name dialog"),tr("Enter unigue node name"),QLineEdit::Normal,node_name);
        if(!text.isEmpty())
        {
            if(FA->renameState(this->node_name,text))
            {
                changeName(text);
                emit FA_changed(FA);
            }
            else
            {
                QString message = tr("Node with same name already exists!");
                errorMessage.showMessage(message);
                errorMessage.exec();
                emit sendStatusBarMessage(message);

            }
        }
    }

    //QGraphicsItem::mouseDoubleClickEvent(event);
}

void StateNode::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu myContextMenu;
    //create checkboxes
    QCheckBox *setStartStateCheckbox = new QCheckBox(tr("start state"),&myContextMenu);
    QCheckBox *setFinalStateCheckbox = new QCheckBox(tr("final state"),&myContextMenu);

    setStartStateCheckbox->setChecked(myscene->startingState == this);
    setFinalStateCheckbox->setChecked(endingState);

    connect(setStartStateCheckbox, SIGNAL(toggled(bool)), this,SLOT(setStartinState()));
    connect(setFinalStateCheckbox, SIGNAL(toggled(bool)), this,SLOT(setEndingState(bool)));

    //create QWidgetAction
    QWidgetAction *setStartStateAction = new QWidgetAction(&myContextMenu);
    QWidgetAction *setFinalStateAction = new QWidgetAction(&myContextMenu);

    //Set widgets to actions
    setStartStateAction->setDefaultWidget(setStartStateCheckbox);
    setFinalStateAction->setDefaultWidget(setFinalStateCheckbox);

    //Add action to menu
    myContextMenu.addAction(setStartStateAction);
    myContextMenu.addAction(setFinalStateAction);

    //show menu
    myContextMenu.exec(event->screenPos());
}



QPainterPath StateNode::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

//void StateNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//}


QRectF StateNode::recalculateTextSpace() const
{
    //prevzato z c++ GUI programming wiht Qt 4, str. 203
    QFontMetrics metrics(qApp->font());
    QRectF rect = metrics.boundingRect(node_name);
    //aby byli kolecka a elipsy stejne vysoke
    if(rect.height()<2*radius)
        rect.setHeight(2*radius);
    rect.adjust(-padding - NODE_PEN_WIDTH / 2,-padding - NODE_PEN_WIDTH / 2,padding+ NODE_PEN_WIDTH,padding+ NODE_PEN_WIDTH);
    rect.translate(-rect.center());
    return rect;
}

QRectF StateNode::elipseBoundingRect() const
{
    QRectF textRect = recalculateTextSpace();
    QRectF defaultRect = QRectF(-radius - NODE_PEN_WIDTH / 2, -radius - NODE_PEN_WIDTH / 2,
                  radius*2 + NODE_PEN_WIDTH, radius*2 + NODE_PEN_WIDTH);
    defaultRect.adjust(-padding,-padding,padding,padding);
    if(textRect.width() > defaultRect.width())
    {
        return textRect;
    }
    else
    {
        return defaultRect;
    }
}

bool StateNode::changeName(QString new_name)
{
    //if(!isNameUnique(new_name))
    //{
    //    return false;
    //}

    node_name = new_name;
    update();
    return true;
}

void StateNode::setNameWithoutCheck(QString node_name)
{
    this->node_name = node_name;
    update();
}

QString StateNode::getName()
{
    return node_name;
}

bool StateNode::isNameUnique(QString s)
{
    return FA->isStateUnique(s);
}



