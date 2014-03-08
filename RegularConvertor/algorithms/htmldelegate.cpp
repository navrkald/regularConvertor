#include "htmldelegate.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <qdebug.h>
#include <QObject>

#define DEFAUL_FONT 9
#define DEFAUL_FONT_FAMILY "Sans Serif"

HTMLDelegate::HTMLDelegate(QObject *parent) : QItemDelegate(parent)
{
    myFont = QFont(DEFAUL_FONT_FAMILY, DEFAUL_FONT);
}

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    //for debug
    //const QRect rect( option.rect );
    //painter->drawLine( rect.topLeft(), rect.topRight() );
    //painter->drawLine( rect.bottomLeft(), rect.bottomRight() );

    QString text = index.model()->data(index, Qt::EditRole).toString();
    if(text != "")
    {
        QSize delegate_size = this->sizeHint(option, index);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::TextAntialiasing, true);

        QRect text_rect = option.rect;

        //
        //Brakepoint
        //
        QRect checkBoxRect = QRect(option.rect.left(),option.rect.top(),0,0);
        if ((index.model()->data(index, Algorithm::HasBrakepoint_Role).toBool()))
        {

                checkBoxRect = option.rect;
                checkBoxRect.setRight((checkBoxRect.left() + 2*myFont.pointSize()));
                drawBrakepoint (painter, option, checkBoxRect, static_cast<bool>(index.model()->data(index, Algorithm::Brakepoint_Role).toBool()));
                text_rect.setLeft(text_rect.left() + checkBoxRect.width());

        }

        //
        //Drawing text:
        //
        QTextDocument document;
        document.setDefaultFont(myFont);
        document.setHtml(text);
        int doc_height =  document.size().height();
        painter->translate(text_rect.topLeft());
        painter->translate(0,+(((delegate_size.height()-doc_height))/2));
        document.drawContents(painter);
        //
        //Picture
        //
        painter->translate(document.size().width(),-+(((delegate_size.height()-doc_height))/2));
        if(index.model()->data(index, Qt::DecorationRole).isValid())
        {
            QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
            QList<QSize> sizes = icon.availableSizes();
            QSize iconsize = sizes.first();
            painter->drawPixmap(0.0, 0.0, icon.pixmap(iconsize));
        }
        painter->restore();
    }
}

QSize HTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

    //doc size
    QTextDocument doc;
    doc.setHtml(index.model()->data(index, Qt::EditRole).toString());
    QSize doc_size = doc.size().toSize();

    //icon size
    QSize icon_size = QSize(0,0);
    if(index.model()->data(index, Qt::DecorationRole).isValid())
    {
        QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
        QList<QSize> sizes = icon.availableSizes();
        icon_size = sizes.first();
    }

    //brakepoin size
    QSize brakepoint_size(0,0);
    if ((index.model()->data(index, Algorithm::HasBrakepoint_Role).toBool()))
    {
        brakepoint_size = QSize(myFont.pointSize()+2*my_margin,myFont.pointSize()+2*my_margin);
    }


    //total height
    QList <int> tmp_list;
    tmp_list << doc_size.height() << icon_size.height() << brakepoint_size.height();
    qSort(tmp_list.begin(), tmp_list.end());
    int total_height = tmp_list.last();

    //total width
    int total_width = brakepoint_size.width() + doc_size.width() + icon_size.width();

    qDebug() << "------------------------";
    qDebug() << "total_height" <<total_height;
    qDebug() << "icon_size.height()" <<icon_size.height();
    qDebug() << "option.rect.height()." << option.rect.height();

    return QSize(total_width,total_height);
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
    //myRect.setRight((myRect.left() + 2*myFont.pointSize()));
    //painter->drawRect(myRect);
    if (!selected)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawEllipse(myRect.center(),myFont.pointSize()/2,myFont.pointSize()/2);
    }
    else
    {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
        painter->drawEllipse(myRect.center(),myFont.pointSize()/2,myFont.pointSize()/2);
    }
}
