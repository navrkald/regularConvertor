#include "variableswidget.h"
#include "ui_variableswidget.h"

CVariablesWidget::CVariablesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CVariablesWidget)
{
    ui->setupUi(this);
}

CVariablesWidget::~CVariablesWidget()
{
    delete ui;
}
