#include "removeepsilonruleswidget.h"
#include "ui_removeepsilonruleswidget.h"

CRemoveEpsilonRulesWidget::CRemoveEpsilonRulesWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CRemoveEpsilonRulesWidget)
{
    ui->setupUi(this);
}

void CRemoveEpsilonRulesWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    ;
}

void CRemoveEpsilonRulesWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    ;
}

CRemoveEpsilonRulesWidget::~CRemoveEpsilonRulesWidget()
{
    delete ui;
}
