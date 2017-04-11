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
    void SetInputFA(FiniteAutomata& FA);
    void SetOutputFA(FiniteAutomata& FA);
    const FiniteAutomata& GetInputFA();
    const FiniteAutomata& GetOutputFA();
private:
    Ui::CFADeterminizationWidget *ui;
    FaToDFA m_dfaAlgorithm;
    AlgorithmModes m_mode;
};

#endif // FADETERMINIZATIONWIDGET_H
