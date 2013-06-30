#include "fa_widget.h"
#include "ui_fa_widget.h"
#include <QRegExpValidator>
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

    //completer part
    //statesStringListModel = new QStringListModel();
    //endingStatesCompleter = new QCompleter(statesStringListModel, this);
    QStringList l;
    l << "ahoj" << "svete";
    endingStatesCompleter = new MultiSelectCompleter(l, this);

    //endingStatesCompleter->setCompletionPrefix(",");
    ui->endingStatesLineEdit->setCompleter(endingStatesCompleter); //to autocomplete ending states

    connect( deleteShortCut, SIGNAL(activated()), scene, SLOT(deleteSelected()));

    //to connect edit buttons with slots
    connect(this->MoveNodeBut,SIGNAL(clicked()), this, SLOT(MoveNodeBut_clicked()));
    connect(this->AddNodeBut,SIGNAL(clicked()),this, SLOT(AddNodeBut_clicked()));
    connect(this->AddArrowBut, SIGNAL(clicked()),this, SLOT(AddArrowBut_clicked()));
    connect(this->DeleteNodeBut, SIGNAL(clicked()), scene, SLOT(deleteSelected()));

    //
    connect(this->ui->statesLineEdit,SIGNAL(editingFinished()),this,SLOT(statesEdited()));
    connect(this->ui->endingStatesLineEdit,SIGNAL(editingFinished()),this,SLOT(endingStatesEdited()));

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
    #if defined(_WIN64) || defined(_WIN32)
        hlayout->addSpacing(18);
    #endif
    vlayout->addLayout(hlayout);
    vlayout->addStretch();

    setupValidators();
}

void FA_widget::setupValidators()
{
    //QRegExp onlyIdentifier("^\\w+$");
    //QRegExp listOfIdentifiers("^\\w+(,\\s*\\w+)*$");
    QRegExp listOfIdentifiers("^\\w+(,\\s*\\w+)*,?$");
    //hint [^,] mean what ever char not ','
    QRegExp listOfSymbols("^\\S(,\\s*\\S)*$");
    QRegExp emptyRegExp("^$");
    statesValidator = new QRegExpValidator(listOfIdentifiers, this);
    alphabetValidator = new QRegExpValidator(listOfSymbols, this);
    endingStatesValidator = new QRegExpValidator(emptyRegExp,this);
    //ui->endingStatesLineEdit->setValidator(endingStatesValidator);

    //startStateValidator = new QRegExpValidator(onlyIdentifier, this);
    //QValidator *endingStatesValidator;
    //QValidator *rules;
    ui->statesLineEdit->setValidator(statesValidator);
    ui->alphabetLineEdit->setValidator(alphabetValidator);
    //ui->startStateLineEdit->setValidator(startStateValidator);
}

QStringList FA_widget::getSortedUniqueList(QString raw_text)
{
    //this list can contain duplicite states
    //first replace sequence of white spaces by one space
    //second remove spaces
    //second split to strings

    //if ends with ',' then remove it
    if(raw_text.endsWith(','))
        raw_text.remove(raw_text.length()-1,1);
    QStringList raw_list_of_states = raw_text.simplified().replace(" ", "" ).split(",");
    QStringList unique_list_of_states = raw_list_of_states.toSet().toList();
    unique_list_of_states.sort();
    return unique_list_of_states;
}

FA_widget::~FA_widget()
{
    delete ui;
}

//sets moving mode
void FA_widget::MoveNodeBut_clicked()
{
    scene->setMode(DiagramScene::MoveNode);
}

//sets adding mode
void FA_widget::AddNodeBut_clicked()
{

    scene->setMode(DiagramScene::AddNode);
}

//sets adding arrow mode
void FA_widget::AddArrowBut_clicked()
{

    scene->setMode(DiagramScene::AddArrow);
}

void FA_widget::DeleteNodeBut_clicked(){
    //scene->setMode(DiagramScene::DeleteNode);
    //delete selected items
    //TODO: at se vymazi v mazaci metode sceny
}

void FA_widget::statesEdited()
{
    QStringList unique_list_of_states = getSortedUniqueList(ui->statesLineEdit->text());
    ui->statesLineEdit->setText(unique_list_of_states.join(", "));
    FA->states = unique_list_of_states.toSet();
    updateStates();
}

void FA_widget::endingStatesEdited()
{
    QStringList SortedUniqueList = getSortedUniqueList(ui->endingStatesLineEdit->text());
    QSet <QString> setOfEndigStates = SortedUniqueList.toSet().intersect(FA->states);
    QStringList listOfEndingStates = setOfEndigStates.toList();
    listOfEndingStates.sort();
    ui->endingStatesLineEdit->setText(listOfEndingStates.join(", "));
    FA->finalStates = setOfEndigStates;
}

void FA_widget::updateStates()
{
    //update start item
    ui->startStateComboBox->clear();
    QList <QString> items = FA->states.toList();
    qSort(items.begin(),items.end());
    ui->startStateComboBox->addItems(items);
    int indexOfCombobox = ui->startStateComboBox->findText(FA->starState);
    ui->startStateComboBox->setCurrentIndex(indexOfCombobox);
    if(FA->starState == "")
    {
        ui->startStateComboBox->setCurrentIndex(0);
        FA->starState = ui->startStateComboBox->itemText(0);
    }

    //statesStringListModel->setStringList(items);
    endingStatesCompleter->setItems(items);
    //set up new values for validator od ending states line edit
    QStringList items_string_list = items;
    QString awailableStates = items_string_list.join( "|" );
    awailableStates = "(" + awailableStates + ")";
    //^\\w+(,\\s*\\w+)*,?$
    endingStatesValidator->setRegExp(QRegExp("^" + awailableStates + "+(,\\s*" + awailableStates + ")*,?$"));

            //QRegExp emptyRegExp("^$");
            //QRegExpValidator* validator= new QRegExpValidator(QRegExp);// QRegExp( sl.join( "|" ) ) );
        //lineEdit->setValidator( validator );
    //validator.setRegExp();
    //validator->setRegExp();
}



//void FA_widget::on_startStateLineEdit_textChanged(const QString &arg1)
//{
//    if()
//}


void FA_widget::on_startStateComboBox_currentIndexChanged(const QString &arg1)
{
    //if(arg1 != "")
    //    FA->starState = QString(arg1);
    ;
}

void FA_widget::on_startStateComboBox_activated(const QString &arg1)
{
    if(arg1 != "")
        FA->starState = QString(arg1);
}
