#include "algorithmwidget.h"
#include "ui_algorithmwidget.h"

AlgorithmWidget::AlgorithmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmWidget)
{
    ui->setupUi(this);
}

AlgorithmWidget::~AlgorithmWidget()
{
    delete ui;
}

AlgorithmView *AlgorithmWidget::getAlgorithmView()
{
    return ui->treeView;
}
