#ifndef CFGTOPDAWIDGET_H
#define CFGTOPDAWIDGET_H

#include <QWidget>
#include <widgets/centralwidgetinterface.h>

namespace Ui {
class CCfgToPdaWidget;
}

class CCfgToPdaWidget : public ICentralCoversionWidget
{
    Q_OBJECT

public:
    explicit CCfgToPdaWidget(QWidget *parent = 0);
    ~CCfgToPdaWidget();

private:
    Ui::CCfgToPdaWidget *ui;

    // ICentralCoversionWidget interface

public:
    void ConnectChangeMode(const QObject *sender, void *modeChanged(Algorithm::modes));
    void ConnectStatusMessage(const QObject *sender, const QMetaMethod &signal);
};

#endif // CFGTOPDAWIDGET_H
