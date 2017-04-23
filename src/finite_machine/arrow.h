#ifndef ARROW_H
#define ARROW_H

#define TEXT_DISTANCE 10
#define LINES_DISTANCE 10

#include <QGraphicsLineItem>
#include <QtWidgets>
#include "statenode.h"
#include "finiteautomata.h"
#include "symbolsinputdialog.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class StateNode;
class DiagramScene;

class Arrow : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
protected:
	//This constructor is usen only in CPdaArrow (implicitely)
    Arrow(StateNode *startItem, StateNode *endItem, QGraphicsItem *parent = 0, DiagramScene *m_scene = 0);

public:

    ~Arrow();
    enum { Type = UserType + 2 };

    Arrow(StateNode *startItem, StateNode *endItem, FiniteAutomata* _FA, QStringList symbol,
    QGraphicsItem *parent = 0, DiagramScene *m_scene = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color) { m_myColor = color; }
    StateNode *startItem() const { return m_myStartItem; }
    StateNode *endItem() const { return m_myEndItem; }
    void addSymbol(QString symbol);
    //return true if there are no aditonal symbols
    bool removeSymbol(QString symbol);
    void updatePosition(); 

public:
    StateNode *m_myStartItem;
    StateNode *m_myEndItem;
    QStringList m_symbols;
    DiagramScene *m_scene;

    virtual void EditArrow();
signals:
    void FA_changed(FiniteAutomata* m_FA);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget = 0);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
        FiniteAutomata* m_FA;

protected:
    QString m_displayText;
    QColor m_myColor;
    QPolygonF m_arrowHead;
    QPolygonF m_selfArrowHead;
    long int m_debugCounter;
    virtual QRect GetDisplayTextRect() const;
    QPolygonF SelfArrowHead();
    QPointF GetDistancePoint() const;
    //Between same nodes are arrows with oposit direction
    bool ArrowHasSibling() const;
    QPointF GetStartItemPos() const;
    QPointF GetEndItemPos() const;
    //pro vypocet jakym smerem se ma sipka posunout, je to proto, aby se opacne sipky mezi stejnymi
    //uzly neprekrivali
    QPointF PerpendicularDifference(QLineF line, qreal distance)const;
    QSize GetDisplayTextSize() const;
    QPointF EllipseLineIntersection(QRectF elipse, QPointF p1, QPointF p2) const;
};

#endif


