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

void CVariablesWidget::SetText(QString text){
    ui->m_text->setText(text);
}

void CVariablesWidget::Clear(){
    ui->m_text->clear();
}
