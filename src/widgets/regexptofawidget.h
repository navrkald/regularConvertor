#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>

namespace Ui {
class CRegExpToFaWidget;
}

class CRegExpToFaWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CRegExpToFaWidget(QWidget *parent = 0);
    ~CRegExpToFaWidget();

private:
    Ui::CRegExpToFaWidget *ui;

    // ICentralCoversionWidget interface
public:
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) override;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) override;
};

#endif // REGEXPTOFA_H
