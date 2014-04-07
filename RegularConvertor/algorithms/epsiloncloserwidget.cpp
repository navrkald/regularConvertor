#include "algorithms/epsiloncloserwidget.h"

EpsilonCloserWidget::EpsilonCloserWidget(QString _state, QList<QString> _epsilon_closer_list, QWidget *parent):
    QWidget(parent), state(_state), epsilon_closer_list(_epsilon_closer_list)
{
    QLabel* label1 = new QLabel("ɜ-uzávěr(" + state + ") = {",this);
    QLabel* label2 = new QLabel("}", this);
    QTextEdit* text_edit = new QTextEdit(state,this);
    //unknown_icon.addFile(":/algorithms/algorithms/pictures/unknown.png");
    correct_icon.addFile(":/algorithms/algorithms/pictures/ok.png");
    wrong_icon.addFile(":/algorithms/algorithms/pictures/wrong.png");
    icon = unknown_icon;
    layout = new QHBoxLayout(this);
    layout->addWidget(label1);
    layout->addWidget(text_edit);
    layout->addWidget(label2);
    QPixmap* map = new QPixmap(":/algorithms/algorithms/pictures/unknown.png");
    QLabel* label = new QLabel(this);
    label->setPixmap(*map);
    layout->addWidget(label);
    //layout->add;
}

void EpsilonCloserWidget::setIsCorrect(bool correct)
{

}
