#include "regexptofawidget.h"
#include "ui_regexptofawidget.h"
#include <mainwindow.h>

CRegExpToFaWidget::CRegExpToFaWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CRegExpToFaWidget)
{
    ui->setupUi(this);
    ui->m_algorithmWidget->SetCaption("Algorithm RegExp to FA");
    ui->m_faLeftSon->SetCaption("Left son");
    ui->m_faChoosenNode->SetCaption("Chosen node");
    ui->m_faRightSon->SetCaption("Right son");
    m_regExpToFaAlgorithm.Init(ui->m_algorithmWidget, ui->m_regExpWidget, ui->m_faLeftSon, ui->m_faChoosenNode, ui->m_faRightSon);
}

CRegExpToFaWidget::~CRegExpToFaWidget()
{
    delete ui;
}

void CRegExpToFaWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_regExpToFaAlgorithm, &RegExpToFA::SendStatusBarMessage, receiver, showMessage);
}

void CRegExpToFaWidget::SetRegExp(RegExp *re)
{
    m_regExpToFaAlgorithm.SetInputRegExp(re);
}

void CRegExpToFaWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_algorithmWidget, &CAlgorithmWidget::SetMode);
    connect(sender, modeChanged, &m_regExpToFaAlgorithm, &RegExpToFA::SetMode);
}


