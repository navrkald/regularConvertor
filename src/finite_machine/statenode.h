#ifndef STATENODE_H
#define STATENODE_H


#define NODE_RADIUS 15.0
#define NODE_PEN_WIDTH 1.0
#define ENDING_STATE_PAADING 10
#define START_STATE_ARROW_LENTH 40
#define START_STATE_ARROW_PEN_WIDTH 5

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

    StateNode(DiagramScene* scene, QString uniqueName);
    ~StateNode();
    Arrow* hasArrowTo(StateNode* node_to);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    double getRadius();
    void addArrow(Arrow *arrow);
    void removeArrows();
    void removeArrow(Arrow *arrow);
    bool changeName(QString new_name);
    void setNameWithoutCheck(QString m_nodeName);
    QRectF elipseBoundingRect() const;
    QList<Arrow *> arrows;
    QString getName();
    QErrorMessage errorMessage;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
signals:
    void deleteItem();
    void sendStatusBarMessage(QString message);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public slots:
    void setStartingState();
    void setEndingState(bool _endingState);
private slots:
    void positionChanged();
private:
    //methods
    void firstInit();
    QRectF recalculateTextSpace() const;
    QStringList getAllNodenames();


    //properties
    static unsigned int ID_counter;
    static const int padding = 8;
    QString m_nodeName;

    //QGraphicsTextItem* text;
    DiagramScene* myscene;
    double radius;
    QBrush* nodeBrush;
    QPen   nodePen;
    bool selected;
    bool pressed;
    bool endingState;

};
#endif // STATENODE_H
