#ifndef REMOVEEPSILONRULES_H
#define REMOVEEPSILONRULES_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>
namespace Ui {
class CRemoveEpsilonRulesWidget;
}

class CRemoveEpsilonRulesWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CRemoveEpsilonRulesWidget(QWidget *parent = 0);
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) = 0;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) = 0;
    ~CRemoveEpsilonRulesWidget();

private:
    Ui::CRemoveEpsilonRulesWidget *ui;
};

#endif // REMOVEEPSILONRULES_H
