#ifndef REMOVEEPSILONRULES_H
#define REMOVEEPSILONRULES_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>
namespace Ui {
class CRemoveEpsilonRules;
}

class CRemoveEpsilonRules : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CRemoveEpsilonRules(QWidget *parent = 0);
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) = 0;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) = 0;
    ~CRemoveEpsilonRules();

private:
    Ui::CRemoveEpsilonRules *ui;
};

#endif // REMOVEEPSILONRULES_H
