#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "algorithms/htmldelegate.h"
#include "algorithms/algorithmview.h"
#include "algorithms/algorithmwidget.h"
//#include "finite_machine/finiteautomata.h"

#define MY_WINDOW_TITLE "Regular Convertor"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    statusBarTimeout = 5000; //5 second
    setWindowTitle(MY_WINDOW_TITLE);
    connect(ui->action_RE_to_FA,SIGNAL(triggered()),this,SLOT(prepareREtoFA()));


    QActionGroup* modesGroup = new QActionGroup(this);
    modesGroup->addAction(ui->action_check_mode);
    modesGroup->addAction(ui->action_play_mode);
    modesGroup->addAction(ui->action_step_mode);

    QActionGroup* conversionGroup = new QActionGroup(this);
    conversionGroup->addAction(ui->action_RE_to_FA);
    conversionGroup->addAction(ui->actionKA_KA_bez_epsilon_p_echod);

    reg_exp_algorithm = 0;
    activeConversion = none;
    mode = Algorithm::PLAY_MODE;

    alhgorithm_widget = new AlgorithmWidget(mode,this);
    alhgorithm_widget->hide();
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));

    //    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
    //    connect( deleteShortCut, SIGNAL(activated()), FA1_widget->scene, SLOT(deleteSelected()));
    //    connect(FA1_widget,SIGNAL(errorMessageSignal(QString)),this, SLOT(myStatusbarShowMessage(QString)));
    //    connect(FA1_widget->scene,SIGNAL(sendErrorMessage(QString)),this,SLOT(myStatusbarShowMessage(QString))
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myStatusbarShowMessage(QString message)
{
    this->ui->statusBar->showMessage(message,statusBarTimeout);
}

void MainWindow::prepareREtoFA(RegExp* _re)
{
    if(activeConversion != RE_to_FA)
    {

        activeConversion = RE_to_FA;
        ui->textBrowser->hide();

        setWindowTitle(MY_WINDOW_TITLE + tr(" - Převod regulárního výrazu na konečný automat"));
        //* layout  = dynamic_cast<QGridLayout*> (ui->centralWidget->layout());
        ui->centralWidget->layout()->setMargin(0);
        QSplitter* h_spitter1 = new QSplitter(Qt::Horizontal,this);
        QSplitter* h_spitter2 = new QSplitter(Qt::Horizontal,this);
        //QSplitter* h_spitter3 = new QSplitter(Qt::Horizontal,this);
        QSplitter* v_spitter1 = new QSplitter(Qt::Vertical,this);

        //basic components
        FA_widget* fa_widget_left = new FA_widget();
        FA_widget* fa_widget_center = new FA_widget();
        FA_widget* fa_widget_right = new FA_widget();
        RegExpWidget* reg_exp_widget = new RegExpWidget(this);

        //initialize
        //reg_exp_widget->setRegExp(_re);

        //regular expression
        QWidget* reg_exp_container = new QWidget();
        QVBoxLayout* reg_exp_vlayout = new QVBoxLayout;
        reg_exp_vlayout->setMargin(0);
        QLabel* reg_exp_label = new QLabel("<b>Regulární výraz</b>",this);
        reg_exp_label->setAlignment(Qt::AlignCenter);
        reg_exp_vlayout->addWidget(reg_exp_label);
        reg_exp_vlayout->addWidget(reg_exp_widget);
        reg_exp_container->setLayout(reg_exp_vlayout);

        //algorithm
        QWidget* algorithm_container = new QWidget();
        QVBoxLayout* algorithm_vlayout = new QVBoxLayout;
        algorithm_vlayout->setMargin(0);
        QLabel* algorithm_label = new QLabel("<b>Algoritmus RV na FA</b>",this);
        algorithm_label->setAlignment(Qt::AlignCenter);
        //alhgorithm_widget = new AlgorithmWidget(mode);
        alhgorithm_widget->show();
        //AlgorithmView* algorithm_RE_to_FA = new AlgorithmView(ui->centralWidget);
        algorithm_vlayout->addWidget(algorithm_label);
        algorithm_vlayout->addWidget(alhgorithm_widget);
        algorithm_container->setLayout(algorithm_vlayout);
        reg_exp_algorithm = new RegExpToFA(alhgorithm_widget, mode, reg_exp_widget, fa_widget_left, fa_widget_center, fa_widget_right, _re);
        HTMLDelegate* delegate = new HTMLDelegate();
        alhgorithm_widget->getAlgorithmView()->setModel(reg_exp_algorithm);
        alhgorithm_widget->getAlgorithmView()->setItemDelegate(delegate);
        connect(delegate,SIGNAL(dataChanged(QModelIndex)),reg_exp_algorithm,SLOT(getData(QModelIndex)));
        connect(this,SIGNAL(modeChanged(Algorithm::modes)),reg_exp_algorithm,SLOT(setMode(Algorithm::modes)));

        //alhgorithm_widget->setModel(reg_exp_algorithm);
        //algorithm_RE_to_FA->tree algorithm_RE_to_FA->setItemDelegate(delegate);

        //left FA
        QWidget* left_fa_container = new QWidget();
        QVBoxLayout* FA_left_vlayout = new QVBoxLayout;
        FA_left_vlayout->setMargin(0);
        QLabel* FA_left_label = new QLabel("<b>levý syn</b>",this);
        FA_left_label->setAlignment(Qt::AlignCenter);
        FA_left_vlayout->addWidget(FA_left_label);
        FA_left_vlayout->addWidget(fa_widget_left);
        left_fa_container->setLayout(FA_left_vlayout);

        //center FA
        QWidget* center_fa_container = new QWidget();
        QVBoxLayout* FA_center_vlayout = new QVBoxLayout;
        FA_center_vlayout->setMargin(0);
        QLabel* FA_center_label = new QLabel("<b>vybraný uzel</b>",this);
        FA_center_label->setAlignment(Qt::AlignCenter);
        FA_center_vlayout->addWidget(FA_center_label);
        FA_center_vlayout->addWidget(fa_widget_center);
        center_fa_container->setLayout(FA_center_vlayout);

        //right FA
        QWidget* right_fa_container = new QWidget();
        QVBoxLayout* FA_right_vlayout = new QVBoxLayout;
        FA_right_vlayout->setMargin(0);
        QLabel* FA_right_label = new QLabel("<b>pravý syn</b>",this);
        FA_right_label->setAlignment(Qt::AlignCenter);
        FA_right_vlayout->addWidget(FA_right_label);
        FA_right_vlayout->addWidget(fa_widget_right);
        right_fa_container->setLayout(FA_right_vlayout);


        //top container
        QWidget* up_container = new QWidget();
        QHBoxLayout* horizontal_layout1 = new QHBoxLayout;
        horizontal_layout1->setMargin(0);
        up_container->setLayout(horizontal_layout1);
        up_container->layout()->addWidget(h_spitter1);
        h_spitter1->addWidget(reg_exp_container);
        h_spitter1->addWidget(algorithm_container);

        //down container
        QWidget* down_container = new QWidget();
        h_spitter2->addWidget(left_fa_container);
        h_spitter2->addWidget(center_fa_container);
        h_spitter2->addWidget(right_fa_container);
        QHBoxLayout* horizontal_layout2 = new QHBoxLayout;
        horizontal_layout2->setMargin(0);
        horizontal_layout2->setSpacing(0);
        down_container->setLayout(horizontal_layout2);
        down_container->layout()->addWidget(h_spitter2);

        //vertical splitter
        v_spitter1->addWidget(up_container);
        v_spitter1->addWidget(down_container);
        ui->centralWidget->layout()->addWidget(v_spitter1);


    }
    else
    {
        ;
    }

}



void MainWindow::on_action_check_mode_triggered()
{
    mode = Algorithm::CHECK_MODE;
    ui->action_check_mode->setChecked(true);
    emit modeChanged(mode);
}

void MainWindow::on_action_play_mode_triggered()
{
    mode = Algorithm::PLAY_MODE;
    ui->action_play_mode->setChecked(true);
    emit modeChanged(mode);
}

void MainWindow::on_action_step_mode_triggered()
{
    mode = Algorithm::STEP_MODE;
    ui->action_step_mode->setChecked(true);
    emit modeChanged(mode);
}


/////////////////////////////////////////
/////////////////////////////////////////
// RegExp to FA EXAMPLES
/////////////////////////////////////////
/////////////////////////////////////////

void MainWindow::setRE_FA_example(RegExp *_re)
{
    on_action_check_mode_triggered();
    if(reg_exp_algorithm)
        reg_exp_algorithm->setExample(_re);
    else
        prepareREtoFA(_re);
}

void MainWindow::on_RE_FA_example0_triggered()
{
    setRE_FA_example(new RegExp());
}

void MainWindow::on_RE_FA_example1_triggered()
{
    setRE_FA_example(new RegExp(EPSILON));
}

void MainWindow::on_RE_FA_example2_triggered()
{
    setRE_FA_example(new RegExp("a"));
}

void MainWindow::on_RE_FA_example3_triggered()
{
    setRE_FA_example(new RegExp("ab"));;
}

void MainWindow::on_RE_FA_example4_triggered()
{
    setRE_FA_example(new RegExp("a*"));;
}

void MainWindow::on_RE_FA_example5_triggered()
{
    setRE_FA_example(new RegExp("a+b"));;
}

void MainWindow::on_RE_FA_example6_triggered()
{
    setRE_FA_example(new RegExp("a+b*"));;
}

void MainWindow::on_RE_FA_example7_triggered()
{
    setRE_FA_example(new RegExp("(a+b)*"));;
}

void MainWindow::on_RE_FA_example8_triggered()
{
    setRE_FA_example(new RegExp("((a+b)*+c)*"));;
}

void MainWindow::on_RE_FA_example9_triggered()
{
    setRE_FA_example(new RegExp("(a+b)*(a+c)*"));;
}
