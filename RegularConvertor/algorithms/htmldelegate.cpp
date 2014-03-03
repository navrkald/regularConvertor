#include "htmldelegate.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <qdebug.h>

#define DEFAUL_FONT 9
#define DEFAUL_FONT_FAMILY "Sans Serif"

HTMLDelegate::HTMLDelegate()
{
    myFont = QFont(DEFAUL_FONT_FAMILY, DEFAUL_FONT);
}

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
//    QStyleOptionViewItemV4 optionV4 = option;
//    //initStyleOption(&optionV4, index);

//    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

//    QTextDocument doc;
//    doc.setHtml(optionV4.text);

//    /// Painting item without text
//    optionV4.text = QString();
//    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

//    QAbstractTextDocumentLayout::PaintContext ctx;

//    // Highlighting text if item is selected
//    if (optionV4.state & QStyle::State_Selected)
//        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));

//    QRect textRect     = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
//    QRect checkboxRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator,&optionV4);
//    Qt::CheckState check_state = static_cast<Qt::CheckState> (index.model()->data(index, Qt::CheckStateRole).toInt());
//    //drawCheck (painter, option, checkboxRect, check_state);
//    if(index.model()->data(index, Qt::CheckStateRole).toBool())
//    {
//        QPen pen;
//        pen.setColor(Qt::red);
//        QBrush brush;
//        brush.setColor(Qt::red);
//        painter->setPen(Qt::red);
//        //painter->setBrush(brush);
//        painter->setBrush(Qt::red);
//        //painter->drawEllipse(checkboxRect);
//    }
//    else
//    {
//        QPen pen;
//        pen.setColor(Qt::black);
//        painter->setPen(Qt::black);
//        QBrush brush;
//        brush.setColor(Qt::red);
//        painter->setBrush(Qt::black);
//        //painter->drawEllipse(checkboxRect);
//    }
//    painter->save();
//    painter->translate(textRect.topLeft());
//    painter->setClipRect(textRect.translated(-textRect.topLeft()));
//    doc.documentLayout()->draw(painter, ctx);
//    painter->restore();
    QString text = index.model()->data(index, Qt::EditRole).toString();
    if(text != "")
    {
        painter->save();
        QRect text_rect = option.rect;
        int left = text_rect.left();
        text_rect.setLeft(left + text_rect.height());
        QTextDocument document;
        document.setDefaultFont(myFont);
        qDebug() << "Point size" << document.defaultFont().pointSizeF();
        qDebug() << "Doc height" << document.size().width();
        document.setDocumentMargin(2);
        document.setHtml(text);
        //text_rect.translate();
        painter->translate(text_rect.topLeft());
        document.drawContents(painter);
        painter->translate(-text_rect.topLeft());

        QRect checkBoxRect = option.rect;
        checkBoxRect.setRight(checkBoxRect.height());

        drawCheck (painter, option, checkBoxRect, static_cast<Qt::CheckState> (index.model()->data(index, Qt::CheckStateRole).toInt()) );

        //painter->drawPixmap(text_rect.topLeft(),icon.pixmap());


        //QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
        QIcon icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");
        //QSize iconsize = icon.actualSize();
        QSize iconsize = option.decorationSize;

        //painter->translate(text_rect.topRight());
        //painter->translate(text_rect.topLeft());
        painter->drawPixmap(text_rect.width()+text_rect.height(), 0.0, icon.pixmap(iconsize.width(), iconsize.height()));
        painter->drawPixmap(0.0, 0.0, icon.pixmap(iconsize.width(), iconsize.height()));
        painter->translate(-text_rect.topLeft());
        //painter->translate(-text_rect.topRight());


        painter->restore();
    }
}

QSize HTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
//    QStyleOptionViewItemV4 optionV4 = option;
//    initStyleOption(&optionV4, index);

//    QTextDocument doc;
//    doc.setHtml(optionV4.text);
//    doc.setTextWidth(optionV4.rect.width());
//    QRect checkboxRect     = option.widget->style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &optionV4);
//    int height = doc.size().height() > checkboxRect.height() ?  doc.size().height() : checkboxRect.height();
//    return QSize(doc.idealWidth()+checkboxRect.width(), height);
    QTextDocument doc;
    doc.setHtml(index.model()->data(index, Qt::EditRole).toString());
    //doc.setTextWidth(option.rect.width());
    //QRect checkboxRect     = option.widget->style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option);
    //int height = doc.size().height() > checkboxRect.height() ?  doc.size().height() : checkboxRect.height();
    return QSize(doc.idealWidth()+doc.size().height(), doc.size().height());
}

bool HTMLDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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

void HTMLDelegate::drawCheck ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, Qt::CheckState state ) const
{


    int margin = rect.width() / 5;

    QRect myRect = rect;
    myRect.setBottom(myRect.bottom() - margin);
    myRect.setTop(myRect.top() + margin);
    myRect.setLeft(myRect.left() + margin);
    myRect.setRight(myRect.right() - margin);
//    myRect.setWidth(2);
//    myRect.setHeight(2);
    if (state == Qt::Unchecked)
    {
        painter->setPen(Qt::white);
        painter->setBrush(Qt::white);
        painter->drawEllipse(myRect);
    }
    else
    {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
        painter->drawEllipse(myRect);
    }
}

//void HTMLDelegate::drawCheck ( QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect,      Qt::CheckState state ) const
//{
//    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

//    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter, rect.size(), QRect(option.rect.x() + textMargin, option.rect.y(),option.rect.width() - (textMargin * 2), option.rect.height()));
//    QItemDelegate::drawCheck(painter, option, checkRect, state);
//}
