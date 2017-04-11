#include "regexpwidget.h"
#include "ui_regexpwidget.h"

CRegExpWidget::CRegExpWidget(QWidget *parent) :
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

CRegExpWidget::~CRegExpWidget()
{
    delete ui;
}

void CRegExpWidget::updateView()
{
    ui->treeView->update();
}

void CRegExpWidget::deselectAll()
{
    QModelIndexList indexList = treeView->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        treeView->selectionModel()->select(index,QItemSelectionModel::Deselect);
    }
}

void CRegExpWidget::setRegExp(RegExp *_re)
{
    ui->regExpTextEdit->setRegExp(_re);
    re = _re;
}

void CRegExpWidget::modelChanged()
{
    treeModel->setRootNode(re->m_rootNode);
    ui->treeView->expandAll();
    emit newRegExp(re);
}

void CRegExpWidget::on_pushButton_clicked()
{
    //ui->regExpTextEdit->moveCursor (QTextCursor::End);
    ui->regExpTextEdit->insertPlainText (EPSILON);
    //ui->regExpTextEdit->moveCursor (QTextCursor::End);
    //  ui->regExpTextEdit->
}
