#include "myqgraphictextitem.h"

myQGraphicTextItem::myQGraphicTextItem(QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    _parent = parent;
    setFlags(QGraphicsItem::ItemIsSelectable);
}

void myQGraphicTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsItem::mouseDoubleClickEvent(event);
}

// child items's mouseMoveEvent
void myQGraphicTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
     //_parent->mouseMoveEvent(event);
     QGraphicsItem::mouseMoveEvent(event);
}


void myQGraphicTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //_parent->mousePressEvent(evemt);
    QGraphicsItem::mousePressEvent(event);
}
