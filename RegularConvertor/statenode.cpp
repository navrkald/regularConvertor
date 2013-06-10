#include "statenode.h"
#include <iostream>

int StateNode::type() const
{
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
}

StateNode::StateNode(DiagramScene* scene)
{
    node_name = "jmeno uzlu";
//    text = new QGraphicsTextItem(this);
//    text->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
//    text->setPlainText("toto je text");
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
    return QRectF(-radius - NODE_PEN_WIDTH / 2, -radius - NODE_PEN_WIDTH / 2,
                  radius*2 + NODE_PEN_WIDTH, radius*2 + NODE_PEN_WIDTH);
}

void StateNode::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
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
    if(selected)
    {
        painter->setBrush(*nodeBrush);
        //painter->setPen(nodePen);
        QPen selectedPen(Qt::DashLine);
        painter->setPen(selectedPen);
        painter->drawEllipse(boundingRect());
        painter->drawText(rect,Qt::AlignCenter,node_name);
    }
    else
    {
        painter->setBrush(*nodeBrush);
        painter->setPen(nodePen);
        painter->drawEllipse(boundingRect());
    }
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


QPainterPath StateNode::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

//void StateNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//}
