#include "algorithmwidget.h"
#include "ui_algorithmwidget.h"

AlgorithmWidget::AlgorithmWidget(Algorithm::modes _mode, QWidget *parent) :
    QWidget(parent), mode(_mode),
    ui(new Ui::AlgorithmWidget)
{
    ui->setupUi(this);

    showSolution = true;

    connect(this->ui->nextButton,SIGNAL(clicked()),this,SIGNAL(nextPressed()));
    connect(this->ui->prewButton,SIGNAL(clicked()),this,SIGNAL(prewPressed()));
    connect(this->ui->playButton,SIGNAL(clicked()),this,SLOT(emitPlay()));
    connect(this->ui->stopButton,SIGNAL(clicked()),this,SIGNAL(stopPressed()));
    connect(this->ui->checkButton,SIGNAL(clicked()),this,SIGNAL(checkSolutionPressed()));
    connect(this->ui->beginButton,SIGNAL(clicked()), this, SIGNAL(beginPressed()));
    connect(this->ui->endButton,SIGNAL(clicked()),this, SIGNAL(endPressed()));
    setWidgets(mode);
}

AlgorithmWidget::~AlgorithmWidget()
{
    delete ui;
}

AlgorithmView *AlgorithmWidget::getAlgorithmView()
{
    return ui->treeView;
}



void AlgorithmWidget::hideWidgets(QList<QWidget *> widgets)
{
    foreach(QWidget* w,widgets)
        w->hide();
}

void AlgorithmWidget::showWidgets(QList<QWidget *> widgets)
{
    foreach(QWidget* w,widgets)
        w->show();
}

void AlgorithmWidget::setWidgets(Algorithm::modes mode)
{

    switch (mode) {
    case Algorithm::CHECK_MODE:
        showWidgets(QList<QWidget*>() << ui->checkButton <<  ui->showButton);
        showSpacer(ui->checkSpacer);
        hideWidgets(QList<QWidget*>()
                    << ui->prewButton << ui->nextButton << ui->playButton << ui->stopButton << ui->spinBox << ui->delay_label << ui->beginButton << ui->endButton
                    << ui->prew_stepButton << ui->next_stepButton );
        hideSpacer(ui->stepSpacer);
        break;
    case Algorithm::PLAY_MODE:
        showWidgets(QList<QWidget*>() << ui->prewButton << ui->nextButton << ui->playButton << ui->stopButton << ui->spinBox << ui->delay_label << ui->beginButton << ui->endButton);
        hideWidgets(QList<QWidget*>()
                    << ui->checkButton <<  ui->showButton
                    << ui->prew_stepButton << ui->next_stepButton );
        hideSpacer(ui->stepSpacer);
        hideSpacer(ui->checkSpacer);
        break;
    case Algorithm::STEP_MODE:
        showWidgets(QList<QWidget*>()<< ui->prew_stepButton << ui->next_stepButton);
        hideWidgets(QList<QWidget*>()
                    << ui->checkButton <<  ui->showButton
                    << ui->prewButton << ui->nextButton << ui->playButton << ui->stopButton << ui->spinBox << ui->delay_label << ui->beginButton << ui->endButton);
        showSpacer(ui->stepSpacer);
        hideSpacer(ui->checkSpacer);
        break;
    default:
        break;
    }
    hideWidgets(QList<QWidget*>()  << ui->prew_stepButton << ui->next_stepButton);
}

void AlgorithmWidget::disableNext()
{
    ui->nextButton->setEnabled(false);
}

void AlgorithmWidget::enableNext()
{
    ui->nextButton->setEnabled(true);
}

void AlgorithmWidget::enablePrev()
{
    ui->prewButton->setEnabled(true);
}

void AlgorithmWidget::disablePrev()
{
    ui->prewButton->setEnabled(false);
}

void AlgorithmWidget::hideSpacer(QSpacerItem *s)
{
   s->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void AlgorithmWidget::showSpacer(QSpacerItem *s)
{
    s->changeSize(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void AlgorithmWidget::enableShowButton()
{
    ui->showButton->setEnabled(true);
}

void AlgorithmWidget::disableShowButton()
{
    ui->showButton->setEnabled(false);
}

void AlgorithmWidget::emitPlay()
{
    emit playPressed(this->ui->spinBox->value());
}

void AlgorithmWidget::on_showButton_clicked()
{
    showSolution=!showSolution;
    if(showSolution)
    {
        ui->showButton->setText(tr("Show solution"));
        emit showUserSolutionPressed();
    }
    else
    {
        ui->showButton->setText(tr("Back"));
        emit showCorrectSolutionPressed();
    }
}
