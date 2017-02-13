#ifndef FADETERMINIZATIONWIDGET_H
#define FADETERMINIZATIONWIDGET_H

#include <QWidget>
#include <algorithms/fatodfa.h>
#include <widgets/centralwidgetinterface.h>

namespace Ui {
class CFADeterminizationWidget;
}

class CFADeterminizationWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CFADeterminizationWidget(QWidget *parent = 0);
    ~CFADeterminizationWidget();
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged);
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage);

private:
    Ui::CFADeterminizationWidget *ui;
    FaToDFA m_DFA_algorithm;
    Algorithm::modes m_mode;
};

#endif // FADETERMINIZATIONWIDGET_H
