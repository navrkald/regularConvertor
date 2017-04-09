#ifndef REGEXPTOFAWIDGET_H
#define REGEXPTOFAWIDGET_H

#include <QWidget>
#include <algorithms/regexptofa.h>
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
    RegExpToFA m_regExpToFaAlgorithm;

public:
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) override;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) override;
};

#endif // REGEXPTOFAWIDGET_H
