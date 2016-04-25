#include "cfgwidget.h"
#include "ui_cfgwidget.h"

CCfgWidget::CCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCfgWidget)
{
    ui->setupUi(this);
}

CCfgWidget::~CCfgWidget()
{
    delete ui;
}
