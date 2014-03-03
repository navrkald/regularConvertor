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
//TODO: dodelat tlacitka dovnitr
//http://doc.qt.digia.com/qt/qgraphicsproxywidget.html#details
//https://www.google.com/search?q=QGraphicsProxyWidget&aq=f&oq=QGraphicsProxyWidget&sourceid=chrome&ie=UTF-8#hl=cs&tbo=d&sclient=psy-ab&q=qgraphicsproxywidget+position+upper+right&oq=qgraphicsproxywidget+position+upper+right&gs_l=serp.3...15672.20351.0.20431.12.12.0.0.0.0.84.838.12.12.0...0.0...1c.1.UjcavTFbebs&pbx=1&bav=on.2,or.r_gc.r_pw.r_cp.r_qf.&fp=bf1270eeb39e6808&bpcl=40096503&biw=1738&bih=824
//    sceneBut = new QToolButton;

//    scene.addWidget(sceneBut);
    //FiniteAutomata* FA_1 = new FiniteAutomata(this);
    //FiniteAutomata* FA_2 = new FiniteAutomata(this);
    //scene1 = new DiagramScene(FA_1, this);
    //scene2 = new DiagramScene(FA_2, this);
    //ui->graphicsView1->setScene(scene1);
    //ui->graphicsView2->setScene(scene2);
    //ui->graphicsView1->show();
    //ui->graphicsView2->show();

    statusBarTimeout = 5000; //5 second
    setWindowTitle(MY_WINDOW_TITLE);
    connect(ui->action_RE_to_FA,SIGNAL(triggered()),this,SLOT(prepareREtoFA()));
    ui->action_RE_to_FA->triggered(true);
    ui->action_play_mode->triggered(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testing_slot()
{
    qDebug() <<"Testing slot launched!";
}

void MainWindow::myStatusbarShowMessage(QString message)
{
    this->ui->statusBar->showMessage(message,statusBarTimeout);
}

void MainWindow::prepareREtoFA()
{
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

    //regular expression
    QWidget* reg_exp_container = new QWidget();
    QVBoxLayout* reg_exp_vlayout = new QVBoxLayout;
    reg_exp_vlayout->setMargin(0);
    QLabel* reg_exp_label = new QLabel("Regulární výraz",this);
    reg_exp_label->setAlignment(Qt::AlignCenter);
    reg_exp_vlayout->addWidget(reg_exp_label);
    reg_exp_vlayout->addWidget(reg_exp_widget);
    reg_exp_container->setLayout(reg_exp_vlayout);

    //algorithm
    QWidget* algorithm_container = new QWidget();
    QVBoxLayout* algorithm_vlayout = new QVBoxLayout;
    algorithm_vlayout->setMargin(0);
    QLabel* algorithm_label = new QLabel("Algoritmus RV na FA",this);
    algorithm_label->setAlignment(Qt::AlignCenter);
    AlgorithmWidget* alhgorithm_widget = new AlgorithmWidget();
    //AlgorithmView* algorithm_RE_to_FA = new AlgorithmView(ui->centralWidget);
    algorithm_vlayout->addWidget(algorithm_label);
    algorithm_vlayout->addWidget(alhgorithm_widget);
    algorithm_container->setLayout(algorithm_vlayout);
    RegExpToFA* reg_exp_algorithm = new RegExpToFA(mode, reg_exp_widget, fa_widget_left, fa_widget_center, fa_widget_right);
    HTMLDelegate* delegate = new HTMLDelegate();
    alhgorithm_widget->getAlgorithmView()->setModel(reg_exp_algorithm);
    alhgorithm_widget->getAlgorithmView()->setItemDelegate(delegate);
    //alhgorithm_widget->setModel(reg_exp_algorithm);
    //algorithm_RE_to_FA->tree algorithm_RE_to_FA->setItemDelegate(delegate);

    //left FA
    QWidget* left_fa_container = new QWidget();
    QVBoxLayout* FA_left_vlayout = new QVBoxLayout;
    FA_left_vlayout->setMargin(0);
    QLabel* FA_left_label = new QLabel("levý syn",this);
    FA_left_label->setAlignment(Qt::AlignCenter);
    FA_left_vlayout->addWidget(FA_left_label);
    FA_left_vlayout->addWidget(fa_widget_left);
    left_fa_container->setLayout(FA_left_vlayout);

    //center FA
    QWidget* center_fa_container = new QWidget();
    QVBoxLayout* FA_center_vlayout = new QVBoxLayout;
    FA_center_vlayout->setMargin(0);
    QLabel* FA_center_label = new QLabel("vybraný uzel",this);
    FA_center_label->setAlignment(Qt::AlignCenter);
    FA_center_vlayout->addWidget(FA_center_label);
    FA_center_vlayout->addWidget(fa_widget_center);
    center_fa_container->setLayout(FA_center_vlayout);

    //right FA
    QWidget* right_fa_container = new QWidget();
    QVBoxLayout* FA_right_vlayout = new QVBoxLayout;
    FA_right_vlayout->setMargin(0);
    QLabel* FA_right_label = new QLabel("pravý syn",this);
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




//    AlgorithmView* listView = new AlgorithmView(this);
//    listView->setMouseTracking(true);


//    QStandardItemModel * model = new QStandardItemModel(10,1);
//    HTMLDelegate* delegate = new HTMLDelegate();
//    listView->setModel(model);
//    listView->setItemDelegate(delegate);
//    model->insertRows(4,4,QModelIndex());
//    for(int i = 0; i < 4; i++)
//    {

//        QModelIndex index = model->index(i,0,QModelIndex());
//        model->setData(index,"a<sup>2</sup><sub style='position: relative; left: -.5em;'>i</sub?",Qt::DisplayRole);
//        model->setData(index, Qt::Unchecked, Qt::CheckStateRole);

//    }
//

//    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
//    connect( deleteShortCut, SIGNAL(activated()), FA1_widget->scene, SLOT(deleteSelected()));


//    connect(FA1_widget,SIGNAL(errorMessageSignal(QString)),this, SLOT(myStatusbarShowMessage(QString)));
//    connect(FA1_widget->scene,SIGNAL(sendErrorMessage(QString)),this,SLOT(myStatusbarShowMessage(QString)));

}



void MainWindow::on_action_check_mode_triggered()
{
    mode = Algorithm::CHECK_MODE;
}

void MainWindow::on_action_play_mode_triggered()
{
    mode = Algorithm::PLAY_MODE;
}

void MainWindow::on_action_step_mode_triggered()
{
    mode = Algorithm::STEP_MODE;
}
