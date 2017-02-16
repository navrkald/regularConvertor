#include "algorithmwidget.h"
#include "ui_algorithmwidget.h"

CAlgorithmWidget::CAlgorithmWidget(QWidget *parent) : QWidget(parent), mode(CAlgorithm::modes::NONE),
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
    ui->Formal->removeTab(1);
}

CAlgorithmWidget::CAlgorithmWidget(CAlgorithm::modes _mode, QWidget *parent) :
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
    ui->Formal->removeTab(1);
}

CAlgorithmWidget::~CAlgorithmWidget()
{
    delete ui;
}

AlgorithmView *CAlgorithmWidget::getAlgorithmView()
{
    return ui->treeView;
}

void CAlgorithmWidget::SetCaption(const QString &caption)
{
    ui->m_algorithmWidgetCaption->setText(caption);
}



void CAlgorithmWidget::hideWidgets(QList<QWidget *> widgets)
{
    foreach(QWidget* w,widgets)
        w->hide();
}

void CAlgorithmWidget::showWidgets(QList<QWidget *> widgets)
{
    foreach(QWidget* w,widgets)
        w->show();
}

void CAlgorithmWidget::setWidgets(CAlgorithm::modes mode)
{

    switch (mode) {
    case CAlgorithm::CHECK_MODE:
        showWidgets(QList<QWidget*>() << ui->checkButton <<  ui->showButton);
        showSpacer(ui->checkSpacer);
        hideWidgets(QList<QWidget*>()
                    << ui->prewButton << ui->nextButton << ui->playButton << ui->stopButton << ui->spinBox << ui->delay_label << ui->beginButton << ui->endButton
                    << ui->prew_stepButton << ui->next_stepButton );
        hideSpacer(ui->stepSpacer);
        break;
    case CAlgorithm::PLAY_MODE:
        showWidgets(QList<QWidget*>() << ui->prewButton << ui->nextButton << ui->playButton << ui->stopButton << ui->spinBox << ui->delay_label << ui->beginButton << ui->endButton);
        hideWidgets(QList<QWidget*>()
                    << ui->checkButton <<  ui->showButton
                    << ui->prew_stepButton << ui->next_stepButton );
        hideSpacer(ui->stepSpacer);
        hideSpacer(ui->checkSpacer);
        break;
    case CAlgorithm::STEP_MODE:
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

void CAlgorithmWidget::disableNext()
{
    ui->nextButton->setEnabled(false);
}

void CAlgorithmWidget::enableNext()
{
    ui->nextButton->setEnabled(true);
}

void CAlgorithmWidget::enablePrev()
{
    ui->prewButton->setEnabled(true);
}

void CAlgorithmWidget::disablePrev()
{
    ui->prewButton->setEnabled(false);
}

void CAlgorithmWidget::hideSpacer(QSpacerItem *s)
{
   s->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void CAlgorithmWidget::showSpacer(QSpacerItem *s)
{
    s->changeSize(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void CAlgorithmWidget::enableShowButton()
{
    ui->showButton->setEnabled(true);
}

void CAlgorithmWidget::disableShowButton()
{
    ui->showButton->setEnabled(false);
}

void CAlgorithmWidget::emitPlay()
{
    emit playPressed(this->ui->spinBox->value());
}

void CAlgorithmWidget::on_showButton_clicked()
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
