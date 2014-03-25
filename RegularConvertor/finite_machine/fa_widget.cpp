#include "fa_widget.h"
#include "ui_fa_widget.h"
#include <QRegExpValidator>
//#include "graphviz.h"
FA_widget::FA_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FA_widget)
{
    ui->setupUi(this);

    FA = new FiniteAutomata();

    scene = new DiagramScene(FA, ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
    deleteShortCut=new QShortcut(QKeySequence::Delete, this);

    //buttons
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


    endingStatesCompleter = new MultiSelectCompleter(this);

    ui->endingStatesLineEdit->setCompleter(endingStatesCompleter); //to autocomplete ending states

    connect( deleteShortCut, SIGNAL(activated()), scene, SLOT(deleteSelected()));

    //to connect edit buttons with slots
    connect(this->MoveNodeBut,SIGNAL(clicked()), this, SLOT(MoveNodeBut_clicked()));
    connect(this->AddNodeBut,SIGNAL(clicked()),this, SLOT(AddNodeBut_clicked()));
    connect(this->AddArrowBut, SIGNAL(clicked()),this, SLOT(AddArrowBut_clicked()));
    connect(this->DeleteNodeBut, SIGNAL(clicked()), this, SLOT(delete_items()));


    //notify changes in formal view
    connect(this->ui->statesLineEdit,SIGNAL(editingFinished()),this,SLOT(statesEdited()));
    connect(this->ui->endingStatesLineEdit,SIGNAL(editingFinished()),this,SLOT(endingStatesEdited()));
    connect(this->ui->alphabetLineEdit,SIGNAL(editingFinished()),this,SLOT(alphaberEdited()));

    //add items to scene
    connect(this,SIGNAL(addNodes(QSet<QString>)),this->scene,SLOT(addNodes(QSet<QString>)));
    connect(this,SIGNAL(removeNodes(QSet<QString>)),this->scene,SLOT(removeNodes(QSet<QString>)));
    connect(this,SIGNAL(setStartNode(QString)),this->scene,SLOT(setStartNode(QString)));
    connect(this,SIGNAL(addEndingNodes(QSet<QString>)),this->scene,SLOT(addEndingNodes(QSet<QString>)));
    connect(this,SIGNAL(removeEndingNodes(QSet<QString>)),this->scene,SLOT(removeEndingNodes(QSet<QString>)));
    connect(this,SIGNAL(addEdges(QSet<ComputationalRules>)),this->scene,SLOT(addEdges(QSet<ComputationalRules>)));
    connect(this,SIGNAL(removeEdges(QSet<ComputationalRules>)),this->scene,SLOT(removeEdges(QSet<ComputationalRules>)));


    //set FA also to scene
    connect(this,SIGNAL(setFA_signal(FiniteAutomata*)),this->scene,SLOT(setFA(FiniteAutomata*)));

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

    //connect(this,SIGNAL(errorMessageSignal(QString)),this,SLOT(testingSlot(QString)));
}

void FA_widget::setupValidators()
{
    QRegExp listOfIdentifiers("^(\\w+(,\\s*\\w+)*,?)?$");
    //hint [^,] mean what ever char not ','
    QRegExp listOfSymbols("^\\S(,\\s*\\S)*$");
    QRegExp emptyRegExp("^$");
    statesValidator = new QRegExpValidator(listOfIdentifiers, this);
    alphabetValidator = new QRegExpValidator(listOfSymbols, this);
    endingStatesValidator = new QRegExpValidator(emptyRegExp,this);
    ui->statesLineEdit->setValidator(statesValidator);
    ui->alphabetLineEdit->setValidator(alphabetValidator);
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

void FA_widget::statesEdited()
{
    //pokud je radek prazdny pak jsme zrejme smazali vsechny uzly
    if(ui->statesLineEdit->text() == "")
    {
        emit removeNodes(FA->states);
        FA->states.clear();
        emit FA_changed(FA);
        return;
    }

    QStringList unique_list_of_states = getSortedUniqueList(ui->statesLineEdit->text());
    ui->statesLineEdit->setText(unique_list_of_states.join(", "));
    QSet <QString> setOfStatesLineEdit = unique_list_of_states.toSet();
    QSet <QString> statesToDel = FA->states - setOfStatesLineEdit;
    QSet <QString> statesToAdd = setOfStatesLineEdit - FA->states;
    FA->states =setOfStatesLineEdit;
    if(!statesToDel.empty())
    {
        emit FA_changed(FA);
        emit removeNodes(statesToDel);
    }
    if(!statesToAdd.empty())
    {
        emit addNodes(statesToAdd);
        emit FA_changed(FA);
    }
    updateStates();
    emit FA_changed(FA);
}

void FA_widget::endingStatesEdited()
{
    QStringList SortedUniqueList = getSortedUniqueList(ui->endingStatesLineEdit->text());
    QSet <QString> setOfEndigStates = SortedUniqueList.toSet().intersect(FA->states);
    QStringList listOfEndingStates = setOfEndigStates.toList();
    listOfEndingStates.sort();
    ui->endingStatesLineEdit->setText(listOfEndingStates.join(", "));
    QSet <QString> endingStatesToDel = FA->finalStates - setOfEndigStates;
    QSet <QString> endingStatesToAdd = setOfEndigStates - FA->finalStates;
    FA->finalStates = setOfEndigStates;
    if(!endingStatesToDel.empty())
        emit removeEndingNodes(endingStatesToDel);
    if(!endingStatesToAdd.empty())
        emit addEndingNodes(endingStatesToAdd);

    emit FA_changed(FA);
}

void FA_widget::alphaberEdited()
{
    QStringList SortedUniqueList = getSortedUniqueList(ui->alphabetLineEdit->text());
    ui->alphabetLineEdit->setText(SortedUniqueList.join(", "));


    QSet <QString> setOfAlphabet =  SortedUniqueList.toSet();
    QSet <QString> symbolsToDel = FA->alphabet - setOfAlphabet;
    QSet <QString> symbolsToAdd = setOfAlphabet - FA->alphabet;
    FA->alphabet = SortedUniqueList.toSet();
    if(!symbolsToDel.empty())
        emit removeSymbols(symbolsToDel);
    if(!symbolsToAdd.empty())
        emit addSymbols(symbolsToAdd);

    emit FA_changed(FA);
    //TODO! osetrit kdyz se vyskytuji symboly abecedy v prechodech
}

void FA_widget::updateStates()
{
    //This blok of code update combobox with start state
    ui->startStateComboBox->clear();    
    QList <QString> items = FA->states.toList();
    qSort(items.begin(),items.end());
    ui->startStateComboBox->addItems(items);
    int indexOfCombobox = ui->startStateComboBox->findText(FA->startState);
    ui->startStateComboBox->setCurrentIndex(indexOfCombobox);
    if(FA->startState == "")
    {//if start state wasn't defined define it
        ui->startStateComboBox->setCurrentIndex(0);
        FA->startState = ui->startStateComboBox->itemText(0);
        emit setStartNode(FA->startState);
    }

    //This block of code update ending state validator and completer
    endingStatesCompleter->setItems(items);
    //set up new values for validator od ending states line edit
    QStringList items_string_list = items;
    QString awailableStates = items_string_list.join( "|" );
    awailableStates = "(" + awailableStates + ")";
    //^\\w+(,\\s*\\w+)*,?$
    endingStatesValidator->setRegExp(QRegExp("^" + awailableStates + "+(,\\s*" + awailableStates + ")*,?$"));
    //TODO! osetrit kdyz se smaze uzel ktery se vyskytuje v prechodech
}

//zmena startovniho stavu ve formalnim popisu
void FA_widget::on_startStateComboBox_activated(const QString &arg1)
{
    statesEdited(); //zavolame explicitne protoze "strati fokus, tak aby to fungovalo"
    if(arg1 != "")
    {
        FA->startState = QString(arg1);
        emit setStartNode(arg1);
    }
    emit FA_changed(FA);
}

//DOTO kdyz jsou prazdne uzly tak vyhodit pri pokusu pridavat prechody error message

//pridani noveho pravidla ve dormalnim popisu FA
void FA_widget::on_addRuleToolButton_clicked()
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphaberEdited();

    if(FA->states.isEmpty())
    {
        QString message = tr("Nelse pridavat prechody. Musite prvne pridat nejake uzly!");
        errorMessage.showMessage(message);
        errorMessage.exec();
        emit errorMessageSignal(message);
        emit FA_changed(FA);
        return;
    }

    //Predavani serazenych listu
    QStringList states_list = FA->states.toList();
    QStringList alphabet_list = FA->alphabet.toList();
    states_list.sort();
    alphabet_list.sort();
    editRuleDialog* ruleEditWindow = new editRuleDialog(states_list,alphabet_list,this);

    if(ruleEditWindow->exec())
    {
        QString from = ruleEditWindow->getFrom();
        QString symbol = ruleEditWindow->getSymbol();
        QString to = ruleEditWindow->getTo();
        if(FA->addRule(ComputationalRules(from,to, symbol)))
        {
            ui->rulesListWidget->addItem(from + " " + symbol + " -> " + to);
            QSet <ComputationalRules> rules_set;
            rules_set.insert(ComputationalRules(from,to,symbol));
            emit addEdges(rules_set);
        }
        else
        {
            ;//TODO vypsat warning
        }
    }
    emit FA_changed(FA);
}

//TODO vypisovat warningy na statusbar
//TODO dodelat errorove messages pomoci privatniho obektu errorMessage

//odstranění pravidla z formalniho popisu FA
void FA_widget::on_removeRuleToolButton_clicked()
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphaberEdited();

    QList<QListWidgetItem *> items = ui->rulesListWidget->selectedItems();

    QSet <ComputationalRules> rules_to_del;
    foreach(QListWidgetItem* i, items)
    {
        ComputationalRules rule_to_del(i->text());
        rules_to_del.insert(rule_to_del);
        FA->removeRule(rule_to_del);
    }
    emit removeEdges(rules_to_del);
    qDeleteAll(ui->rulesListWidget->selectedItems());
    emit FA_changed(FA);

}

//editace pravidel ve formalnim popisu FA
void FA_widget::on_rulesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphaberEdited();

    ComputationalRules oldrule(item->text());

    //Predavani serazenych listu
    QStringList states_list = FA->states.toList();
    QStringList alphabet_list = FA->alphabet.toList();
    states_list.sort();
    alphabet_list.sort();

    editRuleDialog* ruleEditWindow = new editRuleDialog(states_list,alphabet_list,this);
    ruleEditWindow->setFrom(oldrule.from);
    ruleEditWindow->setSymbol(oldrule.symbol);
    ruleEditWindow->setTo(oldrule.to);
    if(ruleEditWindow->exec())
    {
        ComputationalRules newrule(ruleEditWindow->getFrom(),ruleEditWindow->getTo(),ruleEditWindow->getSymbol());
        //pokud se jedna o nove pravidlo
        if(FA->changeRule(oldrule,newrule))
        {
            //remove old rule
            QSet <ComputationalRules> delete_rules_set;
            delete_rules_set.insert(oldrule);
            emit removeEdges(delete_rules_set);

            //add new rule
            QSet <ComputationalRules> new_rules_set;
            new_rules_set.insert(newrule);
            emit addEdges(new_rules_set);

            QListWidgetItem* selectedItem = ui->rulesListWidget->currentItem();
            selectedItem->setText(newrule.toString());
        }
        else
        {
            ;//TODO item se nezmenil
        }
    }
    emit FA_changed(FA);
}

//preklopi se formalni popis do grafu
void FA_widget::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        QStringList states = FA->states.toList();
        QStringList alphabet = FA->alphabet.toList();
        //QList <ComputationalRules> rules = FA->rules.toList();
        //QString startState = FA->starState;
        QStringList endingStates = FA->finalStates.toList();


        ui->statesLineEdit->setText(states.join(", "));
        updateStates(); //also set start state too
        ui->alphabetLineEdit->setText(alphabet.join(", "));
        ui->rulesListWidget->clear();
        foreach(ComputationalRules r,FA->rules)
        {
            ui->rulesListWidget->addItem(r.from + " " + r.symbol + " -> " + r.to);
        }
        ui->endingStatesLineEdit->setText(endingStates.join(", "));

    }
    emit FA_changed(FA);
}

//vymaze selected items
void FA_widget::delete_items()
{
    this->scene->deleteSelected();
    emit FA_changed(FA);
}

//vymaze formalni popis FA
void FA_widget::clean()
{
    ui->alphabetLineEdit->setText("");
    ui->statesLineEdit->setText("");
    ui->endingStatesLineEdit->setText("");
    ui->rulesListWidget->clear();
    ui->startStateComboBox->clear();
    emit FA_changed(FA);
}


//nastavý nový automat
void FA_widget::setFA(FiniteAutomata* FA)
{
    clean();
    this->FA = FA;
    emit setFA_signal(FA);
    emit FA_changed(FA);
}
