#include "epsiloncloserwidget.h"

EpsilonCloserWidget::EpsilonCloserWidget(QString _state, QList<QString> _epsilon_closer_list, QWidget *parent) :
    QWidget(parent), state(_state), epsilon_closer_list(_epsilon_closer_list)
{
    QLabel* label1 = new QLabel("ɜ-uzávěr(" + state + ") = {",this);
    QLabel* label2 = new QLabel("}", this);
    QTextEdit* text_edit = new QTextEdit(state,this);
}
