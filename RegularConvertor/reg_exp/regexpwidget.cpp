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
    connect(ui->regExpTextEdit,SIGNAL(regExpChanged()),this,SLOT(modelChanged()));
}

RegExpWidget::~RegExpWidget()
{
    delete ui;
}

void RegExpWidget::modelChanged()
{
    treeModel->setRootNode(re->rootNode);
    ui->treeView->expandAll();
}

void RegExpWidget::on_pushButton_clicked()
{
    ui->regExpTextEdit->moveCursor (QTextCursor::End);
    ui->regExpTextEdit->insertPlainText (EPSILON);
    ui->regExpTextEdit->moveCursor (QTextCursor::End);
    //  ui->regExpTextEdit->
}
