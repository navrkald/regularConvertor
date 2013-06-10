#ifndef STATENODE_H
#define STATENODE_H


#define NODE_RADIUS 25.0
#define NODE_PEN_WIDTH 1.0


#include <QGraphicsItem>
#include <QPainter>
#include "arrow.h"
#include "diagramscene.h"

class Arrow;
class DiagramScene;

class StateNode : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    int type() const;

    //StateNode();

    StateNode(DiagramScene* scene);
    //~StateNode();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    double getRadius();
    void addArrow(Arrow *arrow);
    void removeArrows();
    void removeArrow(Arrow *arrow);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void deleteItem();
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString node_name;
//    QGraphicsTextItem* text;
    DiagramScene* myscene;
    double radius;
    QBrush* nodeBrush;
    QPen   nodePen;
    bool selected;
    bool pressed;
    QList<Arrow *> arrows;
};

#endif // STATENODE_H
