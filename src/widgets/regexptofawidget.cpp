#include "regexptofawidget.h"
#include "ui_regexptofawidget.h"
//#include <mainwindow.h>

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


void CRegExpToFaWidget::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
}

void CRegExpToFaWidget::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
}
