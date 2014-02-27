#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "algorithms/htmldelegate.h"
#include "algorithms/algorithmview.h"


//#include "finite_machine/finiteautomata.h"
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

    connect(ui->action_RE_to_FA,SIGNAL(triggered()),this,SLOT(prepareREtoFA()));
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
    //* layout  = dynamic_cast<QGridLayout*> (ui->centralWidget->layout());

    QSplitter* h_spitter1 = new QSplitter(Qt::Horizontal,ui->centralWidget);
    QSplitter* v_spitter1 = new QSplitter(Qt::Vertical,this);

    RegExpWidget* reg_exp_widget = new RegExpWidget(ui->centralWidget);
    AlgorithmView* algorithm_RE_to_FA = new AlgorithmView(ui->centralWidget);

    h_spitter1->addWidget(reg_exp_widget);
    h_spitter1->addWidget(algorithm_RE_to_FA);

    reg_exp_widget->show();
    algorithm_RE_to_FA->show();
//    h_spitter1->addWidget();

//    FA1_widget = new FA_widget(this);
//    this->regExpWidget = new RegExpWidget(this);

//    QSplitter * splitter1 = new QSplitter(Qt::Vertical,this);
//    QSplitter * splitter2 = new QSplitter(Qt::Horizontal,this);
//    AlgorithmView* listView = new AlgorithmView(this);
//    listView->setMouseTracking(true);

//    QGridLayout* layout  = dynamic_cast<QGridLayout*> (ui->centralWidget->layout());
//    if(layout != NULL)
//    {
//        splitter1->addWidget(FA1_widget);
//        splitter1->addWidget(regExpWidget);

//        splitter2->addWidget(splitter1);
//        layout->addWidget(splitter2,0,0);
//        splitter2->addWidget(listView);

//    }

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
//    statusBarTimeout = 5000; //5 second

//    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
//    connect( deleteShortCut, SIGNAL(activated()), FA1_widget->scene, SLOT(deleteSelected()));

//    connect( deleteShortCut, SIGNAL(activated()), this, SLOT(testing_slot()));

//    connect(FA1_widget,SIGNAL(errorMessageSignal(QString)),this, SLOT(myStatusbarShowMessage(QString)));
//    connect(FA1_widget->scene,SIGNAL(sendErrorMessage(QString)),this,SLOT(myStatusbarShowMessage(QString)));

}


