#include "statenode.h"
#include <iostream>
#include <QGraphicsItem>

unsigned int StateNode::ID_counter = 1;

int StateNode::type() const
{
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
}

StateNode::StateNode(DiagramScene* scene)
{
    node_name = "jmeno uzlu";

//    mytext = new myQGraphicTextItem(this);
//    mytext->setPlainText(node_name);
//    int x = mytext->x() - mytext->boundingRect().width() / 2;
//    int y = mytext->y() - mytext->boundingRect().height() / 2;
//    mytext->setPos(x, y);

//    text = new QGraphicsTextItem(this);
//    text->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
//    text->setPlainText("toto je text");
//    int x = text->x() - text->boundingRect().width() / 2;
//    int y = text->y() - text->boundingRect().height() / 2;
//    text->setPos(x, y);

    myscene = scene;
    radius = NODE_RADIUS;
    nodeBrush = new QBrush(Qt::yellow);
    //nodeBrush->setColor(Qt::darkBlue);

    nodePen.setColor(Qt::black);
    nodePen.setWidth(NODE_PEN_WIDTH);
    selected = false;
    pressed = false;
//    setFlag(ItemIsMovable);
//    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//StateNode::~StateNode()
//{
//    delete nodeBrush;
//    std::cout << "~test()" << std::endl;
//}

QRectF StateNode::boundingRect() const
{
    QRectF textRect = recalculateTextSpace();
    QRectF defaultRect = QRectF(-radius - NODE_PEN_WIDTH / 2, -radius - NODE_PEN_WIDTH / 2,
                  radius*2 + NODE_PEN_WIDTH, radius*2 + NODE_PEN_WIDTH);
    defaultRect.adjust(-padding,-padding,padding,padding);
    if(textRect.width() > defaultRect.width())
        return textRect;
    else
        return defaultRect;
}

void StateNode::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void StateNode::setStartinState(bool _startingState)
{
    startingState = _startingState;
    update();
}

void StateNode::setEndingState(bool _endingState)
{
    endingState = _endingState;
    update();
}

void StateNode::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        //scene()->removeItem(arrow);
        delete arrow;
    }
}

void StateNode::addArrow(Arrow *arrow)
{
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
        if (value == true)
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

void StateNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawEllipse(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, node_name);
}


void StateNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void StateNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    if(myscene->getMode() == DiagramScene::DeleteNode)
        myscene->emitDeleteSelected();
    //emit myscene->deleteSelected();
}

void StateNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),tr("New node name dialog"),tr("Enter unigue node name"),QLineEdit::Normal,node_name);
    if(!text.isEmpty())
        setName(text);
}

void StateNode::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu myContextMenu;
    //create checkboxes
    QCheckBox *setStartStateCheckbox = new QCheckBox(tr("start state"),&myContextMenu);
    QCheckBox *setFinalStateCheckbox = new QCheckBox(tr("final state"),&myContextMenu);

    setStartStateCheckbox->setChecked(startingState);
    setFinalStateCheckbox->setChecked(endingState);

    connect(setStartStateCheckbox, SIGNAL(toggled(bool)), this,SLOT(setEndingState(bool)));
    connect(setFinalStateCheckbox, SIGNAL(toggled(bool)), this,SLOT(setStartinState(bool)));

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
    QFontMetrics metrics = qApp->font();
    QRectF rect = metrics.boundingRect(node_name);
    //aby byli kolecka a elipsy stejne vysoke
    if(rect.height()<2*radius)
        rect.setHeight(2*radius);
    rect.adjust(-padding - NODE_PEN_WIDTH / 2,-padding - NODE_PEN_WIDTH / 2,padding+ NODE_PEN_WIDTH,padding+ NODE_PEN_WIDTH);
    rect.translate(-rect.center());
    return rect;
}

bool StateNode::setName(QString new_name)
{
    node_name = new_name;
    update();
    //todo
    return true;
}

bool StateNode::isNameUnique(QString s)
{
    //TODO
    return true;
}

//QStringList StateNode::getAllNodenames()
//{
//    //TODO
//    QStringList names;
//    QList<QGraphicsItem *> items = myscene->items();
//    return NULL;
//}
