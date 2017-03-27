#include "regexptofa.h"
#include "ui_regexptofa.h"

CRegExpToFA::CRegExpToFA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRegExpToFA)
{
    ui->setupUi(this);
}

CRegExpToFA::~CRegExpToFA()
{
    delete ui;
}


void CRegExpToFA::ConnectChangeMode(const MainWindow *sender, MainWindowModeChangedMemFn modeChanged)
{
}

void CRegExpToFA::ConnectStatusMessage(const MainWindow *receiver, MainWindowShowStatusMessageFn showMessage)
{
}
