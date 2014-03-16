#include "regexpwidget.h"
#include "ui_regexpwidget.h"

RegExpWidget::RegExpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegExpWidget)
{
    re = new RegExp("",this);
    ui->setupUi(this);
    ui->regExpTextEdit->setRegExp(re);
    this->treeModel = new RegExpTreeModel(this);
    this->ui->treeView->setModel(treeModel);
    modelChanged();
    selectionModel = ui->treeView->selectionModel();
    connect(ui->regExpTextEdit, SIGNAL(regExpChanged()), this ,SLOT(modelChanged()));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SIGNAL(itemClicked(QModelIndex)));
    treeView = ui->treeView;
}

RegExpWidget::~RegExpWidget()
{
    delete ui;
}

void RegExpWidget::updateView()
{
    ui->treeView->update();
}

void RegExpWidget::deselectAll()
{
    QModelIndexList indexList = treeView->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        treeView->selectionModel()->select(index,QItemSelectionModel::Deselect);
    }
}

void RegExpWidget::modelChanged()
{
    emit newRegExp(re);
    treeModel->setRootNode(re->rootNode);
    ui->treeView->expandAll();
}

void RegExpWidget::on_pushButton_clicked()
{
    //ui->regExpTextEdit->moveCursor (QTextCursor::End);
    ui->regExpTextEdit->insertPlainText (EPSILON);
    //ui->regExpTextEdit->moveCursor (QTextCursor::End);
    //  ui->regExpTextEdit->
}
