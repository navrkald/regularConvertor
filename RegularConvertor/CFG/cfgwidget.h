#ifndef CFGWIDGET_H
#define CFGWIDGET_H

#include <QWidget>

namespace Ui {
class CCfgWidget;
}

class CCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CCfgWidget(QWidget *parent = 0);
    ~CCfgWidget();

private:
    Ui::CCfgWidget *ui;
};

#endif // CFGWIDGET_H
