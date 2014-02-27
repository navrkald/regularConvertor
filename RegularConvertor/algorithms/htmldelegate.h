#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>
//#include <QWidget>
#include <QPainter>
#include <QTextDocument>

class HTMLDelegate : public QItemDelegate
{
public:
    HTMLDelegate();

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index);
    //virtual void drawCheck ( QPainter *painter, const QStyleOptionViewItem &option, const QRect & rect, Qt::CheckState state ) const;
    virtual void drawCheck ( QPainter *painter, const QStyleOptionViewItem &option, const QRect &,      Qt::CheckState state ) const;
};
#endif // HTMLDELEGATE_H
