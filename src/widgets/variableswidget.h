#ifndef VARIABLESWIDGET_H
#define VARIABLESWIDGET_H

#include <QWidget>

namespace Ui {
class CVariablesWidget;
}

class CVariablesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CVariablesWidget(QWidget *parent = 0);
    void SetText(QString text);
    void Clear();
    ~CVariablesWidget();

private:
    Ui::CVariablesWidget *ui;
};

#endif // VARIABLESWIDGET_H
