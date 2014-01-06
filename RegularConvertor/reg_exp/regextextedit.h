#ifndef REGEXPTEXTEDIT
#define REGEXPTEXTEDIT


#include <QtGui>
#include <QtWidgets>
#include "reg_exp/regexp.h"

class RegExpTextEdit : public QTextEdit
{
    Q_OBJECT
public:
        RegExpTextEdit(QWidget *parent = 0);
        void setRegExp(RegExp* _re);
        void keyPressEvent(QKeyEvent *event);
        QSize sizeHint() const;
public slots:
        void validateText();
signals:
        void regExpChanged();
private:
        RegExp* re;
        static int staticID;
};

#endif // REGEXPTEXTEDIT
