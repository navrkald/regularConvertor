#include "fadeterminizationwidget.h"
#include "ui_fadeterminizationwidget.h"

CFADeterminizationWidget::CFADeterminizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFADeterminizationWidget)
{
    ui->setupUi(this);
}

CFADeterminizationWidget::~CFADeterminizationWidget()
{
    delete ui;
}
