#include "regexptofawidget.h"
#include "ui_regexptofawidget.h"
#include <mainwindow.h>

CRegExpToFaWidget::CRegExpToFaWidget(QWidget *parent) :
    ICentralCoversionWidget(parent),
    ui(new Ui::CRegExpToFaWidget)
{
    ui->setupUi(this);
}

CRegExpToFaWidget::~CRegExpToFaWidget()
{
    delete ui;
}

void CRegExpToFaWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
    connect(&m_regExpToFaAlgorithm, &RegExpToFA::SendStatusBarMessage, receiver, showMessage);
}

void CRegExpToFaWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
    connect(sender, modeChanged, this->ui->m_algorithmWidget, &CAlgorithmWidget::SetMode);
    connect(sender, modeChanged, &m_regExpToFaAlgorithm, &RegExpToFA::SetMode);
}


