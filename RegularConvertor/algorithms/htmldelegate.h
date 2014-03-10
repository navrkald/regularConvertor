#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>
//#include <QWidget>
#include <QPainter>
#include <QTextDocument>
#include "algorithms/algorithm.h"

class HTMLDelegate : public QItemDelegate
{

    Q_OBJECT

public:
    explicit HTMLDelegate(QObject *parent = 0);
    QFont myFont;
    int my_margin;
    void drawBrakepoint ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, bool selected )const;

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index);



};
#endif // HTMLDELEGATE_Hselectedselected
