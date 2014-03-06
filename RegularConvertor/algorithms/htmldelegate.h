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
public:
    HTMLDelegate();
    QFont myFont;

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    void drawBrakepoint ( QPainter *painter, const QStyleOptionViewItem &option, const QRect &, bool selected ) const;
};
#endif // HTMLDELEGATE_Hselectedselected
