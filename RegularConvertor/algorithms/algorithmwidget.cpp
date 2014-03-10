#include "algorithmwidget.h"
#include "ui_algorithmwidget.h"

AlgorithmWidget::AlgorithmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmWidget)
{
    ui->setupUi(this);
    connect(this->ui->nextButton,SIGNAL(clicked()),this,SIGNAL(nextPressed()));
    connect(this->ui->prewButton,SIGNAL(clicked()),this,SIGNAL(prewPressed()));
    connect(this->ui->playButton,SIGNAL(clicked()),this,SLOT(emitPlay()));
    connect(this->ui->stopButton,SIGNAL(clicked()),this,SIGNAL(stopPressed()));
}

AlgorithmWidget::~AlgorithmWidget()
{
    delete ui;
}

AlgorithmView *AlgorithmWidget::getAlgorithmView()
{
    return ui->treeView;
}

void AlgorithmWidget::emitPlay()
{
    emit playPressed(this->ui->spinBox->value());
}
