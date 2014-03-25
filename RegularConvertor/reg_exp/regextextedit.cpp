#include "regextextedit.h"

int RegExpTextEdit::staticID = 0;

RegExpTextEdit::RegExpTextEdit(QWidget *parent) : QTextEdit(parent)
{
        setTabChangesFocus(true);
        setWordWrapMode(QTextOption::NoWrap);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setFixedHeight(sizeHint().height());

        this->setObjectName(QString("RegExpTextEdit%1").arg(RegExpTextEdit::staticID++));
        connect(this,SIGNAL(textChanged()),this,SLOT(validateText()));
}

void RegExpTextEdit::setRegExp(RegExp *_re)
{
    re = _re;

    disconnect(this,SIGNAL(textChanged()),this,SLOT(validateText()));
    setText(re->regexp);
    connect(this,SIGNAL(textChanged()),this,SLOT(validateText()));
}

void RegExpTextEdit::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
                event->ignore();
        else
                QTextEdit::keyPressEvent(event);
}

QSize RegExpTextEdit::sizeHint() const
{
        QFontMetrics fm(font());
        int h = qMax(fm.height(), 14) + 4;
        int w = fm.width(QLatin1Char('x')) * 17 + 4;
        QStyleOptionFrameV2 opt;
        opt.initFrom(this);
        return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h).
                                          expandedTo(QApplication::globalStrut()), this));
}

void RegExpTextEdit::validateText()
{
    QString text = this->toPlainText();
    if(re->init(text))
    {
        this->setStyleSheet("#" + this->objectName() + "{background: white;}");
        emit regExpChanged();
    }
    else
    {
        this->setStyleSheet("#" + this->objectName() + "{background: red;}");
        //delete re->rootNode;
        //re->rootNode = 0;
        qDebug() << "To red" << this->objectName();
    }

}


