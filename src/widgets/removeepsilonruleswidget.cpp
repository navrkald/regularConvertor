#include "removeepsilonruleswidget.h"
#include "ui_removeepsilonruleswidget.h"
#include <mainwindow.h>

CRemoveEpsilonRulesWidget::CRemoveEpsilonRulesWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CRemoveEpsilonRulesWidget)
{
    ui->setupUi(this);
}

void CRemoveEpsilonRulesWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_removeEpsilonAlgorithm, &RemoveEpsilon::SendStatusBarMessage, receiver, showMessage);
}

void CRemoveEpsilonRulesWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_algorithWidget, &CAlgorithmWidget::SetMode);
    connect(sender, modeChanged, &m_removeEpsilonAlgorithm, &RemoveEpsilon::SetMode);
}

CRemoveEpsilonRulesWidget::~CRemoveEpsilonRulesWidget()
{
    delete ui;
}

void CRemoveEpsilonRulesWidget::SetInputFA(FiniteAutomata &FA)
{
    m_removeEpsilonAlgorithm.SetInputFA(FA);
}

void CRemoveEpsilonRulesWidget::SetOutputFA(FiniteAutomata &FA)
{
    m_removeEpsilonAlgorithm.SetOutputFA(FA);
}

const FiniteAutomata &CRemoveEpsilonRulesWidget::GetInputFA()
{
    return m_removeEpsilonAlgorithm.GetInputFA();
}

const FiniteAutomata &CRemoveEpsilonRulesWidget::GetOutputFA()
{
    return m_removeEpsilonAlgorithm.GetOutputFA();
}
