#include "ui_fawidget.h"
#include "widgets/fawidget.h"
#include <QRegExpValidator>
//#include "graphviz.h"
FA_widget::FA_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FA_widget)
{
    ui->setupUi(this);

    m_finiteAutomata = new FiniteAutomata();

    ui->graphicsView->show();
    deleteShortCut=new QShortcut(QKeySequence::Delete, this);

    //buttons
    MoveNodeBut = new QToolButton();
    AddNodeBut = new QToolButton();
    AddArrowBut = new QToolButton();
    DeleteNodeBut = new QToolButton();

    MoveNodeBut->setIcon(QIcon(":/finite_machine/finite_machine/pictures/cursor.png"));
    AddNodeBut->setIcon(QIcon(":/finite_machine/finite_machine/pictures/add_node.png"));
    AddArrowBut->setIcon(QIcon(":/finite_machine/finite_machine/pictures/add_arrow.png"));
    DeleteNodeBut->setIcon(QIcon(":/finite_machine/finite_machine/pictures/delete.png"));
    //set then checkable
    MoveNodeBut->setCheckable(true);
    AddNodeBut->setCheckable(true);
    AddArrowBut->setCheckable(true);

    //set init checked to AddNodeBut
    AddNodeBut->setChecked(true);
    //MoveNodeBut->setChecked(true);

    MoveNodeBut->setToolTip(tr("If this button is activated, you can select and move with items in scene."));
    AddNodeBut->setToolTip(tr("If this button is activated, left mouse click into screene creates new node."));
    AddArrowBut->setToolTip(tr("If this button is activated, you can create transition between nodes. <br>(Click on from item, hold mouse left button and release on arrow target.)"));
    DeleteNodeBut->setToolTip(tr("Click on this button deletes all selected items. <br>(You can select multiple items when holding control button.)"));

    //adjust size of button according text in
//    MoveNodeBut->setMaximumWidth(MoveNodeBut->fontMetrics().boundingRect(MoveNodeBut->text()).width()+15);
//    AddNodeBut->setMaximumWidth(AddNodeBut->fontMetrics().boundingRect(AddNodeBut->text()).width()+15);
//    AddArrowBut->setMaximumWidth(AddArrowBut->fontMetrics().boundingRect(AddArrowBut->text()).width()+15);
//    DeleteNodeBut->setMaximumWidth(DeleteNodeBut->fontMetrics().boundingRect(DeleteNodeBut->text()).width()+15);


    endingStatesCompleter = new MultiSelectCompleter(this);

    ui->endingStatesLineEdit->setCompleter(endingStatesCompleter); //to autocomplete ending states

    //to connect edit buttons with slots
    connect(this->MoveNodeBut,SIGNAL(clicked()), this, SLOT(MoveNodeBut_clicked()));
    connect(this->AddNodeBut,SIGNAL(clicked()),this, SLOT(AddNodeBut_clicked()));
    connect(this->AddArrowBut, SIGNAL(clicked()),this, SLOT(AddArrowBut_clicked()));
    connect(this->DeleteNodeBut, SIGNAL(clicked()), this, SLOT(delete_items()));


    //notify changes in formal view
    connect(this->ui->statesLineEdit,SIGNAL(editingFinished()),this,SLOT(statesEdited()));
    connect(this->ui->endingStatesLineEdit,SIGNAL(editingFinished()),this,SLOT(endingStatesEdited()));
    connect(this->ui->alphabetLineEdit,SIGNAL(editingFinished()),this,SLOT(alphabetEdited()));
    connect(this->ui->alphabetLineEdit,SIGNAL(textEdited(QString)),this,SLOT(alphabetEdited()));

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
    status_label = new QLabel();
    hlayout->addWidget(status_label);
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

    SetScene(new DiagramScene(m_finiteAutomata, ui->graphicsView));
}

void FA_widget::setupValidators()
{
    QRegExp listOfIdentifiers("^(\\w+(,\\s*\\w+)*,?)?$");
    //hint [^,] mean what ever char not ','
    QRegExp listOfSymbols("^(\\S(,\\s*\\S)*)?$");
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

void FA_widget::setCorrectStatus()
{
    status_label->setText("<b><font color=\"green\">CORRECT</font></b>");
    status_label->show();
}

void FA_widget::setWrongStatus()
{
    status_label->setText("<b><font color=\"red\">WRONG</font></b>");
    status_label->show();
}

void FA_widget::clearStatus()
{
  status_label->hide();
}

void FA_widget::SetSceneGeneral(DiagramScene* scene){
    m_scene = scene;
    connect(m_scene,SIGNAL(SendStatusBarMessage(QString)),this,SIGNAL(SendStatusBarMessage(QString)));
    ui->graphicsView->setScene(m_scene);

    connect( deleteShortCut, SIGNAL(activated()), m_scene, SLOT(deleteSelected()));

    //add items to scene
    connect(this,SIGNAL(addNodes(QSet<QString>)),this->m_scene,SLOT(addNodes(QSet<QString>)));
    connect(this,SIGNAL(removeNodes(QSet<QString>)),this->m_scene,SLOT(removeNodes(QSet<QString>)));
    connect(this,SIGNAL(setStartNode(QString)),this->m_scene,SLOT(setStartNode(QString)));
    connect(this,SIGNAL(addEndingNodes(QSet<QString>)),this->m_scene,SLOT(addEndingNodes(QSet<QString>)));
    connect(this,SIGNAL(removeEndingNodes(QSet<QString>)),this->m_scene,SLOT(removeEndingNodes(QSet<QString>)));

}

void FA_widget::SetSceneSpecific(DiagramScene* scene){
    // Connect rules
    connect(this,SIGNAL(addEdges(QSet<ComputationalRules>)),this->m_scene,SLOT(addEdges(QSet<ComputationalRules>)));
    connect(this,SIGNAL(removeEdges(QSet<ComputationalRules>)),this->m_scene,SLOT(removeEdges(QSet<ComputationalRules>)));

    //set FA also to scene
    connect(this,SIGNAL(setFA_signalToScene(FiniteAutomata*)),this->m_scene,SLOT(SetFa(FiniteAutomata*)));

    // FA changed - scene notify
    connect(m_scene,SIGNAL(FA_changed(FiniteAutomata*)),this,SIGNAL(FA_changed(FiniteAutomata*)));
}

void FA_widget::SetScene(DiagramScene* scene)
{
    SetSceneGeneral(scene);
    SetSceneSpecific(scene);
}

FA_widget::~FA_widget()
{
    delete ui;
}

void FA_widget::SetCaption(const QString &caption)
{
    ui->m_finiteMachineWidgetCaption->setText(caption);
}

//sets moving mode
void FA_widget::MoveNodeBut_clicked()
{
    m_scene->SetMode(DiagramScene::MoveNodeMode);
}

//sets adding mode
void FA_widget::AddNodeBut_clicked()
{

    m_scene->SetMode(DiagramScene::AddNodeMode);
}

//sets adding arrow mode
void FA_widget::AddArrowBut_clicked()
{

		m_scene->SetMode(DiagramScene::AddArrowMode);
}

void FA_widget::statesEdited()
{
    //pokud je radek prazdny pak jsme zrejme smazali vsechny uzly
    if(ui->statesLineEdit->text() == "")
    {
        emit removeNodes(m_finiteAutomata->m_states);
        m_finiteAutomata->removeStates(m_finiteAutomata->m_states);
        //FA->states.clear();

        emit FA_changed(m_finiteAutomata);
        return;
    }

    QStringList unique_list_of_states = getSortedUniqueList(ui->statesLineEdit->text());
    ui->statesLineEdit->setText(unique_list_of_states.join(", "));
    QSet <QString> setOfStatesLineEdit = unique_list_of_states.toSet();
    QSet <QString> statesToDel = m_finiteAutomata->m_states - setOfStatesLineEdit;
    QSet <QString> statesToAdd = setOfStatesLineEdit - m_finiteAutomata->m_states;
    m_finiteAutomata->removeStates(statesToDel);
    foreach(QString state_to_add, statesToAdd)
        m_finiteAutomata->addState(state_to_add);
    if(!statesToDel.empty())
    {
        //remove nodes emits in scene FA_changed()
        emit removeNodes(statesToDel);

        // this block of code removes rules in formal view that consist of
        foreach(QString state_to_del,statesToDel)
        {
            QList<QListWidgetItem *>  items_to_dell = ui->rulesListWidget->findItems(state_to_del+" ",Qt::MatchStartsWith);
            items_to_dell.append(ui->rulesListWidget->findItems(" "+state_to_del,Qt::MatchEndsWith));
            foreach(QListWidgetItem* item,items_to_dell.toSet())
            {
                ui->rulesListWidget->takeItem(ui->rulesListWidget->row(item));
                delete item;
            }
        }

    }
    if(!statesToAdd.empty())
    {
        emit addNodes(statesToAdd);
        emit FA_changed(m_finiteAutomata);
    }
    updateStates();
    // not needed  emit FA_changed(FA)(emit only if there is some changes (add or nemove nodes));
}

void FA_widget::endingStatesEdited()
{
    QStringList SortedUniqueList = getSortedUniqueList(ui->endingStatesLineEdit->text());
    QSet <QString> setOfEndigStates = SortedUniqueList.toSet().intersect(m_finiteAutomata->m_states);
    QStringList listOfEndingStates = setOfEndigStates.toList();
    listOfEndingStates.sort();
    ui->endingStatesLineEdit->setText(listOfEndingStates.join(", "));
    QSet <QString> endingStatesToDel = m_finiteAutomata->m_finalStates - setOfEndigStates;
    QSet <QString> endingStatesToAdd = setOfEndigStates - m_finiteAutomata->m_finalStates;
    m_finiteAutomata->m_finalStates = setOfEndigStates;
    if(!endingStatesToDel.empty())
    {
        emit removeEndingNodes(endingStatesToDel);
        //FA changed is emited in node
    }
    if(!endingStatesToAdd.empty())
    {
        emit addEndingNodes(endingStatesToAdd);
        // FA changed is emited in node
    }
}

void FA_widget::alphabetEdited()
{
    QString text = ui->alphabetLineEdit->text();
    int pos = 0;
    if (alphabetValidator->validate(text, pos) == QValidator::Acceptable)
    {
        QStringList SortedUniqueList = getSortedUniqueList(ui->alphabetLineEdit->text());
        ui->alphabetLineEdit->setText(SortedUniqueList.join(", "));


        QSet <QString> setOfAlphabet =  SortedUniqueList.toSet();
        QSet <QString> symbolsToDel = m_finiteAutomata->m_alphabet - setOfAlphabet;
        QSet <QString> symbolsToAdd = setOfAlphabet - m_finiteAutomata->m_alphabet;
        if(!symbolsToDel.empty())
        {
            foreach(QString symbol, symbolsToDel)
            {
                // This block of code removes rules from formal view where delete symbols appears
                //QList<QListWidgetItem *>  items_to_dell = ui->rulesListWidget->findItems(" "+symbol+" ->",Qt::MatchContains);
                QList<QListWidgetItem *>  items_to_dell = ui->rulesListWidget->findItems("^\\S+ "+symbol+" -> \\S+$",Qt::MatchRegExp);
                foreach(QListWidgetItem* item,items_to_dell.toSet())
                {
                    ui->rulesListWidget->takeItem(ui->rulesListWidget->row(item));
                    delete item;
                }
                emit removeEdges(m_finiteAutomata->findRule_Symbol(symbol).toSet());
                m_finiteAutomata->removeSymbol(symbol);
            }
            emit FA_changed(m_finiteAutomata);
        }

        if(!symbolsToAdd.empty())
        {
            foreach(QString symbol, symbolsToAdd)
            {
                m_finiteAutomata->addSymbol(symbol);
            }
            emit FA_changed(m_finiteAutomata);
        }
    }
    //TODO! osetrit kdyz se vyskytuji symboly abecedy v prechodech
}

void FA_widget::updateStates()
{
    //This blok of code update combobox with start state
    ui->startStateComboBox->clear();    
    QList <QString> items = m_finiteAutomata->m_states.toList();
    qSort(items.begin(),items.end());
    ui->startStateComboBox->addItems(items);
    int indexOfCombobox = ui->startStateComboBox->findText(m_finiteAutomata->m_startState);
    ui->startStateComboBox->setCurrentIndex(indexOfCombobox);
    if(m_finiteAutomata->m_startState == "")
    {//if start state wasn't defined define it
        ui->startStateComboBox->setCurrentIndex(0);
        m_finiteAutomata->m_startState = ui->startStateComboBox->itemText(0);
        emit setStartNode(m_finiteAutomata->m_startState);
    }

    //update ending state validator and completer
    endingStatesCompleter->setItems(items);
    //set up new values for validator od ending states line edit
    QStringList items_string_list = items;
    QString awailableStates = "(" + items_string_list.join( "|" ) + ")";
    endingStatesValidator->setRegExp(QRegExp("^" + awailableStates + "+(,\\s*" + awailableStates + ")*,?$")); // RegExp: ^\\w+(,\\s*\\w+)*,?$
    QStringList endingStates = m_finiteAutomata->m_finalStates.toList();
    ui->endingStatesLineEdit->setText(endingStates.join(", "));
}

void FA_widget::emitAddEdge(ComputationalRules rule)
{
    QSet<ComputationalRules> rules;
    rules.insert(rule);
    emit addEdges(rules);
}

void FA_widget::emitAddEndingNode(QString node)
{
    QSet <QString> nodes;
    nodes.insert(node);
    emit addEndingNodes(nodes);
}

//zmena startovniho stavu ve formalnim popisu
void FA_widget::on_startStateComboBox_activated(const QString &arg1)
{
    statesEdited(); //zavolame explicitne protoze "strati fokus, tak aby to fungovalo"
    //because prew function erase selected item in gui
    int index = ui->startStateComboBox->findText(arg1);
    ui->startStateComboBox->setCurrentIndex(index);
    if(arg1 != "")
    {
        m_finiteAutomata->m_startState = QString(arg1);
        emit setStartNode(arg1);
    }
    emit FA_changed(m_finiteAutomata);
}

//DOTO kdyz jsou prazdne uzly tak vyhodit pri pokusu pridavat prechody error message

//pridani noveho pravidla ve Formalnim popisu FA
void FA_widget::on_addRuleToolButton_clicked()
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphabetEdited();

    if(m_finiteAutomata->m_states.isEmpty())
    {
        QString message = tr("WARNING: Can not add edges. First you have to add some nodes.");
        emit SendStatusBarMessage(message);
        emit FA_changed(m_finiteAutomata);
        return;
    }

    //Predavani serazenych listu
    QStringList states_list = m_finiteAutomata->m_states.toList();
    QStringList alphabet_list = m_finiteAutomata->m_alphabet.toList();
    states_list.sort();
    alphabet_list.sort();
    editRuleDialog* ruleEditWindow = new editRuleDialog(states_list,alphabet_list,this);

    if(ruleEditWindow->exec())
    {
        QString from = ruleEditWindow->getFrom();
        QString symbol = ruleEditWindow->getSymbol();
        QString to = ruleEditWindow->getTo();
        if(m_finiteAutomata->addRule(ComputationalRules(from,to, symbol)))
        {
            ui->rulesListWidget->addItem(from + " " + symbol + " -> " + to);
            QSet <ComputationalRules> rules_set;
            rules_set.insert(ComputationalRules(from,to,symbol));
            emit addEdges(rules_set);
        }
        else
        {
            emit SendStatusBarMessage(tr("WARNING: Your set edge existing."));
        }
    }
    emit FA_changed(m_finiteAutomata);
}

//TODO vypisovat warningy na statusbar
//TODO dodelat errorove messages pomoci privatniho obektu errorMessage

//odstranění pravidla z formalniho popisu FA
void FA_widget::on_removeRuleToolButton_clicked()
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphabetEdited();

    QList<QListWidgetItem *> items = ui->rulesListWidget->selectedItems();

    QSet <ComputationalRules> rules_to_del;
    foreach(QListWidgetItem* i, items)
    {
        ComputationalRules rule_to_del(i->text());
        rules_to_del.insert(rule_to_del);
        m_finiteAutomata->removeRule(rule_to_del);
    }
    emit removeEdges(rules_to_del);
    qDeleteAll(ui->rulesListWidget->selectedItems());
    emit FA_changed(m_finiteAutomata);

}

//editace pravidel ve formalnim popisu FA
void FA_widget::on_rulesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //updatovani stavu a abecedy (zavolame explicitne protoze "nestrati fokus, tak aby to fungovalo")
    statesEdited();
    alphabetEdited();

    ComputationalRules oldrule(item->text());

    //Predavani serazenych listu
    QStringList states_list = m_finiteAutomata->m_states.toList();
    QStringList alphabet_list = m_finiteAutomata->m_alphabet.toList();
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
        if(m_finiteAutomata->changeRule(oldrule,newrule))
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
    emit FA_changed(m_finiteAutomata);
}

//preklopi se formalni popis do grafu
void FA_widget::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        QStringList states = m_finiteAutomata->m_states.toList();
        QStringList alphabet = m_finiteAutomata->m_alphabet.toList();
        //QList <ComputationalRules> rules = FA->rules.toList();
        //QString startState = FA->starState;
        QStringList endingStates = m_finiteAutomata->m_finalStates.toList();


        ui->statesLineEdit->setText(states.join(", "));
        updateStates(); //also set start state too
        ui->alphabetLineEdit->setText(alphabet.join(", "));
        ui->rulesListWidget->clear();
        foreach(ComputationalRules r,m_finiteAutomata->m_rules)
        {
            ui->rulesListWidget->addItem(r.from + " " + r.symbol + " -> " + r.to);
        }
        ui->endingStatesLineEdit->setText(endingStates.join(", "));

    }
    // not needed emit, we set only old data from FA graphic - emit FA_changed(FA);
}

//vymaze selected items
void FA_widget::delete_items()
{
    this->m_scene->DeleteSelected();
    emit FA_changed(m_finiteAutomata);
}

//vymaze formalni popis FA
void FA_widget::clean()
{
    ui->alphabetLineEdit->setText("");
    ui->statesLineEdit->setText("");
    ui->endingStatesLineEdit->setText("");
    ui->rulesListWidget->clear();
    ui->startStateComboBox->clear();
}


//nastavý nový automat
void FA_widget::setFA(FiniteAutomata* FA)
{
    disconnect(m_scene,SIGNAL(FA_changed(FiniteAutomata*)),
               this,SIGNAL(FA_changed(FiniteAutomata*)));
    clean();
    m_finiteAutomata = FA;
    emit setFA_signalToScene(FA);
    connect(m_scene,SIGNAL(FA_changed(FiniteAutomata*)),
            this,SIGNAL(FA_changed(FiniteAutomata*)));
    emit FA_changed(FA);
}
