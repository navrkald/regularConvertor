#ifndef CENTRALWIDGETINTERFACE_H
#define CENTRALWIDGETINTERFACE_H

#include <QWidget>
#include <algorithms/algorithm.h>

class MainWindow;
typedef  void (MainWindow::*MainWindowModeChangedMemFn)(Algorithm::modes);

class ICentralCoversionWidget : public QWidget{
    Q_OBJECT
public:
    explicit ICentralCoversionWidget(QWidget *parent = 0) : QWidget(parent) {}
    virtual void ConnectChangeMode(const QObject *sender, MainWindowModeChangedMemFn modeChanged) = 0;
    virtual void ConnectStatusMessage(const QObject *receiver, const QMetaMethod &method) = 0;
};

#endif // CENTRALWIDGETINTERFACE_H
