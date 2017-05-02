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
    m_cfgToPdaAlgorithm.Init(ui->m_cfgToPdaAlgorithmWidget, ui->m_cfgWidget, ui->m_pdaWidget, ui->m_variablesWidget);
}

CCfgToPdaWidget::~CCfgToPdaWidget()
{
    delete ui;
}

void CCfgToPdaWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_cfgToPdaAlgorithmWidget, &CAlgorithmWidget::SetMode);
    connect(sender, modeChanged, &m_cfgToPdaAlgorithm, &CAlgorithmCFGtoPDA::SetMode);
}

void CCfgToPdaWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_cfgToPdaAlgorithm, &CAlgorithmCFGtoPDA::SendStatusBarMessage, receiver, showMessage);
}

void CCfgToPdaWidget::SetCfg(const CContextFreeGrammar& cfg)
{
    m_cfgToPdaAlgorithm.SetInputCfg(new CContextFreeGrammar(cfg));
}

void CCfgToPdaWidget::SetPda(const CPushDownAutomata &pda)
{
    m_cfgToPdaAlgorithm.SetInputPda(new CPushDownAutomata(pda));   // TODO: Implement
}

CContextFreeGrammar CCfgToPdaWidget::GetCfg()
{
    return m_cfgToPdaAlgorithm.GetCfg();
}

CPushDownAutomata CCfgToPdaWidget::GetPda()
{
    return m_cfgToPdaAlgorithm.GetPda();
}
