#ifndef REMOVEEPSILONRULESWIDGET_H
#define REMOVEEPSILONRULESWIDGET_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>
#include <finite_machine/finiteautomata.h>
#include <algorithms/removeepsilon.h>

namespace Ui {
class CRemoveEpsilonRulesWidget;
}

class CRemoveEpsilonRulesWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CRemoveEpsilonRulesWidget(QWidget *parent = 0);
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged);
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage);
    void SetInputFA(FiniteAutomata& FA);
    void SetOutputFA(FiniteAutomata& FA);
    const FiniteAutomata& GetInputFA();
    const FiniteAutomata& GetOutputFA();
    ~CRemoveEpsilonRulesWidget();

private:
    Ui::CRemoveEpsilonRulesWidget *ui;
    RemoveEpsilon m_removeEpsilonAlgorithm;
    AlgorithmModes m_mode;
};

#endif // REMOVEEPSILONRULESWIDGET_H
