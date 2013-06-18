#include "fa_widget.h"
#include "ui_fa_widget.h"

FA_widget::FA_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FA_widget)
{
    ui->setupUi(this);
    FA = new FiniteAutomata(this);

    scene = new DiagramScene(FA, this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
    deleteShortCut=new QShortcut(QKeySequence::Delete, this);

    MoveNodeBut = new QPushButton("move");
    AddNodeBut = new QPushButton("+");
    AddArrowBut = new QPushButton("arrow");
    DeleteNodeBut = new QPushButton("del");

    //set then checkable
    MoveNodeBut->setCheckable(true);
    AddNodeBut->setCheckable(true);
    AddArrowBut->setCheckable(true);

    //set init checked to AddNodeBut
    AddNodeBut->setChecked(true);

    //adjust size of button according text in
    MoveNodeBut->setMaximumWidth(MoveNodeBut->fontMetrics().boundingRect(MoveNodeBut->text()).width()+15);
    AddNodeBut->setMaximumWidth(AddNodeBut->fontMetrics().boundingRect(AddNodeBut->text()).width()+15);
    AddArrowBut->setMaximumWidth(AddArrowBut->fontMetrics().boundingRect(AddArrowBut->text()).width()+15);
    DeleteNodeBut->setMaximumWidth(DeleteNodeBut->fontMetrics().boundingRect(DeleteNodeBut->text()).width()+15);

    connect( deleteShortCut, SIGNAL(activated()), scene, SLOT(deleteSelected()));

    //to connect edit buttons with slots
    connect(this->MoveNodeBut,SIGNAL(clicked()), this, SLOT(on_MoveNodeBut_clicked()));
    connect(this->AddNodeBut,SIGNAL(clicked()),this, SLOT(on_AddNodeBut_clicked()));
    connect(this->AddArrowBut, SIGNAL(clicked()),this, SLOT(on_AddArrowBut_clicked()));
    connect(this->DeleteNodeBut, SIGNAL(clicked()), scene, SLOT(deleteSelected()));

    //this code displayes edit buttons in front of graphicsView
    QVBoxLayout* vlayout = new QVBoxLayout(ui->graphicsView);
    QHBoxLayout* hlayout = new QHBoxLayout();

    //at one time onli one button can be checked
    QButtonGroup* button_group = new QButtonGroup(vlayout);
    button_group->addButton(AddNodeBut);
    button_group->addButton(MoveNodeBut);
    button_group->addButton(AddArrowBut);
    button_group->setExclusive(true);

    //button positionig
    hlayout->addStretch();
    hlayout->addWidget(MoveNodeBut);
    hlayout->addWidget(AddNodeBut);
    hlayout->addWidget(AddArrowBut);
    hlayout->addWidget(DeleteNodeBut);
    hlayout->setMargin(0);
    vlayout->setMargin(4);
    vlayout->addLayout(hlayout);
    vlayout->addStretch();
}

FA_widget::~FA_widget()
{
    delete ui;
}

//sets moving mode
void FA_widget::on_MoveNodeBut_clicked()
{
    scene->setMode(DiagramScene::MoveNode);
}

//sets adding mode
void FA_widget::on_AddNodeBut_clicked()
{

    scene->setMode(DiagramScene::AddNode);
}

//sets adding arrow mode
void FA_widget::on_AddArrowBut_clicked()
{

    scene->setMode(DiagramScene::AddArrow);
}

void FA_widget::on_DeleteNodeBut_clicked()
{
    //scene->setMode(DiagramScene::DeleteNode);
    //delete selected items
    //TODO: at se vymazi v mazaci metode sceny
}
