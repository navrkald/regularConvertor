#ifndef CENTRALWIDGETINTERFACE_H
#define CENTRALWIDGETINTERFACE_H

#include <QWidget>
#include <algorithms/algorithm.h>

class MainWindow;
typedef void (MainWindow::*MainWindowModeChangedMemFn)(AlgorithmModes);
typedef void (MainWindow::*MainWindowShowStatusMessageFn)(QString);

class ICentralCoversionWidget : public QWidget{
    Q_OBJECT
public:
    explicit ICentralCoversionWidget(QWidget *parent = 0) : QWidget(parent) {}
    virtual void ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged) = 0;
    virtual void ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage) = 0;
};

#endif // CENTRALWIDGETINTERFACE_H
