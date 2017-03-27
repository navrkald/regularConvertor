#include "removeepsilonrules.h"
#include "ui_removeepsilonrules.h"

CRemoveEpsilonRules::CRemoveEpsilonRules(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRemoveEpsilonRules)
{
    ui->setupUi(this);
}

void CRemoveEpsilonRules::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    ;
}

void CRemoveEpsilonRules::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    ;
}

CRemoveEpsilonRules::~CRemoveEpsilonRules()
{
    delete ui;
}
