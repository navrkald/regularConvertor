#ifndef STATENODE_H
#define STATENODE_H


#define NODE_RADIUS 25.0
#define NODE_PEN_WIDTH 1.0

#include <QtCore>
#include <QGraphicsItem>
#include <QPainter>
#include "arrow.h"
#include "diagramscene.h"
#include <QInputDialog>
#include <QtWidgets>
#include "finiteautomata.h"

class Arrow;
class DiagramScene;

class StateNode : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT
    //Q_DECLARE_TR_FUNCTIONS(StateNode)
public:
    enum { Type = UserType + 1 };

    int type() const;

    //StateNode();

    StateNode(DiagramScene* scene, FiniteAutomata* _FA);
    ~StateNode();
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
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
signals:
    void deleteItem();
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public slots:
    void setStartinState(bool _startingState);
    void setEndingState(bool _endingState);

private:
    //methods
    QRectF recalculateTextSpace() const;
    bool changeName(QString new_name);
    bool isNameUnique(QString s);
    QStringList getAllNodenames();

    //properties
    FiniteAutomata* FA;
    static unsigned int ID_counter;
    static const int padding = 8;
    QString node_name;
    //QGraphicsTextItem* text;
    DiagramScene* myscene;
    double radius;
    QBrush* nodeBrush;
    QPen   nodePen;
    bool selected;
    bool pressed;
    bool startingState;
    bool endingState;
    QList<Arrow *> arrows;
};
#endif // STATENODE_H
