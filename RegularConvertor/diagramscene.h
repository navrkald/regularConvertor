#ifndef MYQGRAPHICSSCENE_H
#define MYQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
 #include "arrow.h"

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
signals:
    void deleteSelected();
public:
    DiagramScene(QWidget *parent);
    //~DiagramScene();

    enum Mode { AddNode, MoveNode, DeleteNode, AddArrow};

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    Mode getMode();
    void emitDeleteSelected()
    {emit deleteSelected();}


public slots:
    void setMode(Mode mode);
//private slots:
//    void changeSelected();

private:
    bool clicked;
    Mode actMode;
    QGraphicsLineItem *actLine;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    //    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // MYQGRAPHICSSCENE_H


//#ifndef DIAGRAMSCENE_H
//#define DIAGRAMSCENE_H

//#include <QGraphicsScene>
//#include "diagramitem.h"
//#include "diagramtextitem.h"

//class QGraphicsSceneMouseEvent;
//class QMenu;
//class QPointF;
//class QGraphicsLineItem;
//class QFont;
//class QGraphicsTextItem;
//class QColor;

//class DiagramScene : public QGraphicsScene
//{
//    Q_OBJECT

//public:
//    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

//    DiagramScene(QMenu *itemMenu, QObject *parent = 0);
//    QFont font() const
//        { return myFont; }
//    QColor textColor() const
//        { return myTextColor; }
//    QColor itemColor() const
//        { return myItemColor; }
//    QColor lineColor() const
//        { return myLineColor; }
//    void setLineColor(const QColor &color);
//    void setTextColor(const QColor &color);
//    void setItemColor(const QColor &color);
//    void setFont(const QFont &font);

//public slots:
//    void setMode(Mode mode);
//    void setItemType(DiagramItem::DiagramType type);
//    void editorLostFocus(DiagramTextItem *item);

//signals:
//    void itemInserted(DiagramItem *item);
//    void textInserted(QGraphicsTextItem *item);
//    void itemSelected(QGraphicsItem *item);

//protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

//private:
//    bool isItemChange(int type);

//    DiagramItem::DiagramType myItemType;
//    QMenu *myItemMenu;
//    Mode myMode;
//    bool leftButtonDown;
//    QPointF startPoint;
//    QGraphicsLineItem *line;
//    QFont myFont;
//    DiagramTextItem *textItem;
//    QColor myTextColor;
//    QColor myItemColor;
//    QColor myLineColor;
//};

//#endif
