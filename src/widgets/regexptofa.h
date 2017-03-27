#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>

namespace Ui {
class CRegExpToFA;
}

class CRegExpToFA : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CRegExpToFA(QWidget *parent = 0);
    ~CRegExpToFA();

private:
    Ui::CRegExpToFA *ui;

    // ICentralCoversionWidget interface
public:
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) override;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) override;
};

#endif // REGEXPTOFA_H
