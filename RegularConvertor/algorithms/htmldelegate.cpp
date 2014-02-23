#include "htmldelegate.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>

HTMLDelegate::HTMLDelegate()
{
}

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    QTextDocument doc;
    doc.setHtml(optionV4.text);

    /// Painting item without text
    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;

    // Highlighting text if item is selected
    if (optionV4.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
    QRect checkboxRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator,&optionV4);
    if(index.model()->data(index, Qt::CheckStateRole).toBool())
    {
        QPen pen;
        pen.setColor(Qt::red);
        painter->setPen(pen);
        painter->drawEllipse(checkboxRect);
    }
    else
    {
        QPen pen;
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->drawEllipse(checkboxRect);
    }

    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize HTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setHtml(optionV4.text);
    doc.setTextWidth(optionV4.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

bool HTMLDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ((event->type() == QEvent::MouseButtonRelease) || (event->type() == QEvent::MouseButtonDblClick))
    {
      QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
      if (mouse_event->button() != Qt::LeftButton) {
          return false;
      }
      if (event->type() == QEvent::MouseButtonDblClick) {
        return true;
      }
    }
    else if (event->type() == QEvent::KeyPress)
    {
        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space && static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
        {
            return false;
        }
    }else
    {
      return false;
    }

    bool checked = index.model()->data(index, Qt::CheckStateRole).toBool();
    return model->setData(index, !checked, Qt::CheckStateRole);
}
