#include "algorithms/epsiloncloserwidget.h"

EpsilonCloserWidget::EpsilonCloserWidget(QString _state, QList<QString> _user_epsilon_closer_list, QList<QString> _correct_epsilon_closer_list, QWidget *parent):
    QWidget(parent), state(_state), user_epsilon_closer_list(_user_epsilon_closer_list), correct_epsilon_closer_list(_correct_epsilon_closer_list)
{
    label1 = new QLabel(tr("ɜ-closer(<b>") + state + "</b>) = {",this);
    label2 = new QLabel("}", this);
    line_edit = new QLineEdit(user_epsilon_closer_list.join(", "),this);
    line_edit->show();
    unknown = QPixmap(":/algorithms/algorithms/pictures/unknown.png");
    correct = QPixmap(":/algorithms/algorithms/pictures/ok.png");
    wrong =  QPixmap(":/algorithms/algorithms/pictures/wrong.png");
    layout = new QHBoxLayout(this);
    layout->addWidget(label1);
    layout->addWidget(line_edit);
    layout->addWidget(label2);
    label3 = new QLabel(this);
    label3->setPixmap(unknown.scaledToHeight(iconHeight));
    layout->addWidget(label3);

    connect(line_edit,SIGNAL(textEdited(QString)),this,SLOT(statesEdited(QString)));
    connect(line_edit,SIGNAL(textChanged(QString)),this,SLOT(statesEdited(QString)));
}

void EpsilonCloserWidget::setCorrectness(bool correct)
{
    if(correct)
    {
        label3->setPixmap(this->correct.scaledToHeight(iconHeight));
    }
    else
    {
        label3->setPixmap(this->wrong.scaledToHeight(iconHeight));
    }
}

void EpsilonCloserWidget::setCompleter(MultiSelectCompleter *_completer)
{
    completer = _completer;
    line_edit->setCompleter(completer);
}

void EpsilonCloserWidget::statesEdited(QString s)
{
    user_epsilon_closer_list =  FA_widget::getSortedUniqueList(s);
    if(user_epsilon_closer_list.empty())
        label3->setPixmap(unknown.scaledToHeight(iconHeight));
    else
        setCorrectness(user_epsilon_closer_list == correct_epsilon_closer_list);
}
