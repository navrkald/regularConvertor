#ifndef MYQGRAPHICTEXTITEM_H
#define MYQGRAPHICTEXTITEM_H

#include <QGraphicsTextItem>

class myQGraphicTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    myQGraphicTextItem(QGraphicsItem *parent);


protected:
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QGraphicsItem *_parent;
};

#endif // MYQGRAPHICTEXTITEM_H
