#ifndef EPSILONCLOSERWIDGET_H
#define EPSILONCLOSERWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <set/set_of_sets.h>
#include "finite_machine/multiselectcompleter.h"
#include "widgets/fa_widget.h"

class EpsilonCloserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EpsilonCloserWidget(QString _state, QList<QString> _user_epsilon_closer_list, QList<QString> _correct_epsilon_closer_list, QWidget *parent = 0);

    QString state;
    QStringList user_epsilon_closer_list;
    QStringList correct_epsilon_closer_list;
    QLabel* label1;
    QLabel* label2;
    QLabel* label3;
    QLineEdit* line_edit;
    QPixmap unknown;
    QPixmap correct;
    QPixmap wrong;
    QHBoxLayout* layout;
    const static int iconHeight = 20;
    MultiSelectCompleter* completer;
    QStringList states;

    // Setters
    void setCompleter(MultiSelectCompleter* _completer);
    void setCorrectness(bool correct);

    // Getters
    inline QList<QString> getCloser(){return user_epsilon_closer_list;}
    inline QString getState(){return state;}

signals:

public slots:
    void statesEdited(QString s);
};

#endif // EPSILONCLOSERWIDGET_H
