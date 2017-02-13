#include "cfgtopdawidget.h"
#include "ui_cfgtopdawidget.h"
#include <mainwindow.h>
CCfgToPdaWidget::CCfgToPdaWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CCfgToPdaWidget)
{
    ui->setupUi(this);
    ui->m_pdaWidget->SetCaption("Output: Push-Down Automata");
    ui->m_cfgWidget->SetCaption("Input: Contex-free Grammar");
    ui->m_cfgToPdaAlgorithmWidget->SetCaption("Conversion of CFG to PushdownAutomata");
}

CCfgToPdaWidget::~CCfgToPdaWidget()
{
    delete ui;
}

void CCfgToPdaWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_cfgToPdaAlgorithmWidget, &CAlgorithmWidget::setWidgets);
    connect(sender, modeChanged, &m_cfgToPdaGuiInterface, &CCfgToPdaGuiInterface::setMode);
}

void CCfgToPdaWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_cfgToPdaGuiInterface, &CCfgToPdaGuiInterface::sendStatusBarMessage, receiver, showMessage);
}
