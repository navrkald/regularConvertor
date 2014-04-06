#ifndef EPSILONCLOSERWIDGET_H
#define EPSILONCLOSERWIDGET_H

#include <QWidget>
#include <QtWidgets>
class EpsilonCloserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EpsilonCloserWidget(QString _state, QList<QString> _epsilon_closer_list, QWidget *parent = 0);
    QString state;
    QList<QString> epsilon_closer_list;
    QLabel* label1;
    QLabel* label2;
    QTextEdit* text_edit;
signals:

public slots:

};

#endif // EPSILONCLOSERWIDGET_H
