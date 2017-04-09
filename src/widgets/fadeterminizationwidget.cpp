#include "fadeterminizationwidget.h"
#include "ui_fadeterminizationwidget.h"
#include <mainwindow.h>

CFADeterminizationWidget::CFADeterminizationWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CFADeterminizationWidget)
{
    ui->setupUi(this);
    ui->m_algorithmWidget->SetCaption("FA determinization");
    ui->m_inputFaWidget->SetCaption("Input FA");
    ui->m_outputFaWidget->SetCaption("Output FA");
    m_dfaAlgorithm.Init(ui->m_algorithmWidget, ui->m_inputFaWidget, ui->m_outputFaWidget, ui->m_variablesWidget);
}

CFADeterminizationWidget::~CFADeterminizationWidget()
{
    delete ui;
}

void CFADeterminizationWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_algorithmWidget, &CAlgorithmWidget::SetMode);
    connect(sender, modeChanged, &m_dfaAlgorithm, &FaToDFA::SetMode);
}

void CFADeterminizationWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_dfaAlgorithm, &FaToDFA::SendStatusBarMessage, receiver, showMessage);
}

void CFADeterminizationWidget::SetInputFA(FiniteAutomata &FA)
{
    m_dfaAlgorithm.setInputFA(FA);
}

void CFADeterminizationWidget::SetOutputFA(FiniteAutomata &FA)
{
    m_dfaAlgorithm.setOutputFA(FA);
}

const FiniteAutomata &CFADeterminizationWidget::GetInputFA()
{
    return m_dfaAlgorithm.GetInputFA();
}

const FiniteAutomata &CFADeterminizationWidget::GetOutputFA()
{
    return m_dfaAlgorithm.GetOutputFA();
}
