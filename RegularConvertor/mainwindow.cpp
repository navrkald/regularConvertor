#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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
    scene1 = new DiagramScene(this);
    scene2 = new DiagramScene(this);
    ui->graphicsView1->setScene(scene1);
    ui->graphicsView2->setScene(scene2);
    ui->graphicsView1->show();
    ui->graphicsView2->show();

    deleteShortCut=new QShortcut(QKeySequence::Delete, this);
    connect( deleteShortCut, SIGNAL(activated()), this, SLOT(deleteItem()));
    connect( scene1, SIGNAL(deleteSelected()), this, SLOT(deleteItem()));

    //connect(ui->AddNodeBut,SIGNAL(clicked()), scene,SLOT(setMode(DiagramScene::AddNode)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_AddNodeBut_clicked()
{
    scene1->setMode(DiagramScene::AddNode);
}

void MainWindow::on_MoveNodeBut_clicked()
{
    scene1->setMode(DiagramScene::MoveNode);
}

void MainWindow::deleteItem()
{

    std::cout<<"deleteItem()"<<std::endl;
    foreach (QGraphicsItem *item, scene1->selectedItems())
    {
        std::cout<<"deleteItem() -- foreach"<<std::endl;
        if (item->type() == Arrow::Type) {
            scene1->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene1->selectedItems())
    {
         if (item->type() == StateNode::Type)
         {
             qgraphicsitem_cast<StateNode *>(item)->removeArrows();
         }


         scene1->removeItem(item);
         itemsToDelete.append(item);
         //QTimer::singleShot(1000, this, SLOT(delaidDelete()));
         delete item;
     }

    std::cout<<"deleteItem()   ---- return"<<std::endl;
}

void MainWindow::delaidDelete()
{
    foreach (QGraphicsItem *item, itemsToDelete)
    {
        delete item;
    }
}


void MainWindow::on_DeleteNodeBut_clicked()
{
    scene1->setMode(DiagramScene::DeleteNode);
//    foreach (QGraphicsItem *item, scene1->selectedItems()) {
//        if (item->type() == Arrow::Type) {
//            scene->removeItem(item);
//            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
//            arrow->startItem()->removeArrow(arrow);
//            arrow->endItem()->removeArrow(arrow);
//            delete item;
//        }
//    }
}

void MainWindow::on_AddArrow_clicked(bool checked)
{
    if(checked)
        scene1->setMode(DiagramScene::AddArrow);
}
