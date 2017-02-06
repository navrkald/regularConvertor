#ifndef FADETERMINIZATIONWIDGET_H
#define FADETERMINIZATIONWIDGET_H

#include <QWidget>

namespace Ui {
class CFADeterminizationWidget;
}

class CFADeterminizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CFADeterminizationWidget(QWidget *parent = 0);
    ~CFADeterminizationWidget();

private:
    Ui::CFADeterminizationWidget *ui;
};

#endif // FADETERMINIZATIONWIDGET_H
