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
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::TextAntialiasing, true);

        //
        //Checkbox
        //
        painter->save();
        QRect checkBoxRect = option.rect;
        //checkBoxRect.setRight(myFont.pointSize());
        checkBoxRect.setRight(checkBoxRect.left()+option.rect.height());
        //drawBrakepoint (painter, option, checkBoxRect, static_cast<bool>(index.model()->data(index, Algorithm::Brakepoint_Role).toBool()));
        painter->restore();
        //painter->save();

        QRect text_rect = option.rect;
        text_rect.setLeft(text_rect.left() + text_rect.height());

        //
        //Drawing text:
        //
        QTextDocument document;
        document.setDefaultFont(myFont);
        //qDebug() << "Point size" << document.defaultFont().pointSizeF();
        //qDebug() << "Doc height" << document.size().width();
        document.setHtml(text);
        painter->translate(text_rect.topLeft());
        document.drawContents(painter);
        //painter->translate(-text_rect.topLeft());

        //painter->restore();




        //painter->save();
        //painter->drawPixmap(QPoint(0,0),icon.pixmap());

        //
        //Picture
        //
        //painter->translate(text_rect.topLeft()+QPoint(document.size().width(),0));
        painter->translate(document.size().width(),0);
        //QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
        QIcon icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");
        QList<QSize> sizes = icon.availableSizes();
        QSize iconsize = sizes.first();

        //painter->translate(text_rect.topRight());
        //painter->translate(text_rect.topLeft());


        //painter->drawPixmap(text_rect.width()+text_rect.height(), 0.0, icon.pixmap(iconsize.width(), iconsize.height()));


        painter->drawPixmap(0.0, 0.0, icon.pixmap(iconsize));

        //painter->translate(-text_rect.topLeft());
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

    QIcon icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");
    QList<QSize> sizes = icon.availableSizes();
    QSize iconsize = sizes.first();

    //int margin = 2*QStyle::PM_FocusFrameHMargin;
    int margin = 0;
    return QSize(doc.idealWidth()+doc.size().height()+iconsize.width()+10+margin, doc.size().height()+10+margin);

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

    bool checked = index.model()->data(index, Algorithm::Brakepoint_Role).toBool();
    return model->setData(index, !checked, Algorithm::Brakepoint_Role);
}

void HTMLDelegate::drawBrakepoint ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, bool selected ) const
{
    QRect myRect = rect;
    //qDebug()<< "myRect: " <<myRect.width() <<"," << myRect.height();
    if (!selected)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawEllipse(myRect.center(),myFont.pointSize(),myFont.pointSize());
    }
    else
    {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
        painter->drawEllipse(myRect.center(),myFont.pointSize(),myFont.pointSize());
    }
}
