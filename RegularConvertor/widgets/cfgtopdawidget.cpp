#include "cfgtopdawidget.h"
#include "ui_cfgtopdawidget.h"

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

void CCfgToPdaWidget::ConnectChangeMode(const QObject *sender, void *modeChanged(Algorithm::modes))
{
    ;
}

void CCfgToPdaWidget::ConnectStatusMessage(const QObject *receiver, const QMetaMethod &method)
{
    ;
}
