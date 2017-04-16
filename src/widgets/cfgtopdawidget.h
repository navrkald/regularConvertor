#ifndef CFGTOPDAWIDGET_H
#define CFGTOPDAWIDGET_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>
#include <gui_algorithm_interface/cfgtopdaguiinterface.h>

namespace Ui {
class CCfgToPdaWidget;
}

// Class Context-free grammar to Push down automata
// conversion widget
class CCfgToPdaWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CCfgToPdaWidget(QWidget *parent = 0);
    ~CCfgToPdaWidget();

private:
    Ui::CCfgToPdaWidget *ui;
    CAlgorithmCFGtoPDA m_cfgToPdaAlgorithm;
public:
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged);
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage);
};

#endif // CFGTOPDAWIDGET_H
