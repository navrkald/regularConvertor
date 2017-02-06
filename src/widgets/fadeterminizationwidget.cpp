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
}

CFADeterminizationWidget::~CFADeterminizationWidget()
{
    delete ui;
}

void CFADeterminizationWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_algorithmWidget, &CAlgorithmWidget::setWidgets);
    connect(sender, modeChanged, &m_DFA_algorithm, &FaToDFA::setMode);
}

void CFADeterminizationWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_DFA_algorithm, &FaToDFA::sendStatusBarMessage, receiver, showMessage);
}
