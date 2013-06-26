#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
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

    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
    connect( deleteShortCut, SIGNAL(activated()), ui->FA1_widget->scene, SLOT(deleteSelected()));
    connect( deleteShortCut, SIGNAL(activated()), ui->FA2_widget->scene, SLOT(deleteSelected()));
    connect( deleteShortCut, SIGNAL(activated()), this, SLOT(testing_slot()));
    testing_slot();
    //connect( scene1, SIGNAL(deleteSelected()), this, SLOT(deleteItem()));

    //connect(ui->AddNodeBut,SIGNAL(clicked()), scene,SLOT(setMode(DiagramScene::AddNode)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testing_slot()
{
    qDebug() <<"Testing slot launched!";
}
