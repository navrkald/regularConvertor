#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "algorithms/htmldelegate.h"
#include "algorithms/algorithmview.h"
#include "widgets/algorithmwidget.h"
#include <widgets/cfgtopdawidget.h>
#include <widgets/fadeterminizationwidget.h>
#include <widgets/removeepsilonruleswidget.h>
#include <widgets/regexptofawidget.h>

#define MY_APP_NAME "Regular Convertor"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_centralWidget(NULL)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);

    statusBarTimeout = 5000; //5 second
    setWindowTitle(MY_APP_NAME);
    SetActionsGroups();

    m_activeConversion = none;
    m_mode = AlgorithmModes::algorithmSteping;
    ui->action_play_mode->setChecked(true);

    // Custom status bar
    status_label = new QLabel(this);
    statusBar()->addWidget(status_label);
    status_label->hide();
    status_timer = new QTimer(this);
    status_timer->setInterval(5000);

    status_label->setStyleSheet("QLabel { background-color : white;}");

    translator = new QTranslator(this);

    connect(status_timer,SIGNAL(timeout()),this,SLOT(hideStatusMessage()));

    ui->menuLanguages->menuAction()->setVisible(false);

	connect(this, &MainWindow::modeChangedVoid, this, &MainWindow::OnModeChangedSlot);

	on_CfgToPda_Example_1_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mySetWindowTitle()
{
    QString conversionStr;
    QString modeStr;
    switch(m_activeConversion)
    {
        case none:
            conversionStr="";
        break;
        case RE_to_FA:
            conversionStr=tr("RegExp to Finite autoamta");
        break;
        case REMOVE_EPSILON:
            conversionStr=tr("Remove epsilon rules");
        break;
        case DFA:
            conversionStr=tr("FA determinization");
        break;
    }

    switch(m_mode)
    {
        case AlgorithmModes::NONE:
            modeStr = "";
        break;
        case AlgorithmModes::individualWork:
            modeStr = tr("Mode: individual work");
        break;
        case AlgorithmModes::algorithmSteping:
            modeStr = tr("Mode: algorithm stepping");
        break;
        case AlgorithmModes::instantChecking:
            modeStr = tr("Mode: instant checking");
        break;
    }

    QString exampleStr;
    if(m_exampleName != "")
        exampleStr = " - " + m_exampleName;
    setWindowTitle(MY_APP_NAME " - " + conversionStr + " - " + modeStr + exampleStr);

}

void MainWindow::showStatusMessage(QString message)
{
    message.replace("WARNING:","<b><font color=#FF00FF>WARNING:</font></b>");
    message.replace("ERROR:","<b><font color=\"red\">ERROR:</font></b>");
    status_label->setText(message);
    status_label->show();
    status_timer->start();
}

void MainWindow::hideStatusMessage()
{
    status_timer->stop();
    status_label->hide();
}

void MainWindow::on_action_check_mode_triggered()
{
    m_mode = AlgorithmModes::individualWork;
    ui->action_check_mode->setChecked(true);
    emit modeChangedVoid();
}

void MainWindow::on_action_play_mode_triggered()
{
    m_mode = AlgorithmModes::algorithmSteping;
    ui->action_play_mode->setChecked(true);
    emit modeChangedVoid();
}

void MainWindow::on_action_step_mode_triggered()
{
    m_mode = AlgorithmModes::instantChecking;
    ui->action_step_mode->setChecked(true);
    emit modeChangedVoid();
}

void MainWindow::OnModeChangedSlot()
{
	mySetWindowTitle();
}

void MainWindow::PrepareConversionWidget(MainWindow::Conversions conversion)
{
    m_activeConversion = conversion;
    if(m_centralWidget){
         delete m_centralWidget;
        m_centralWidget = NULL;
    }
    switch(m_activeConversion){
        case RE_to_FA:
            m_centralWidget = new CRegExpToFaWidget(this);
            break;
        case REMOVE_EPSILON:
            m_centralWidget = new CRemoveEpsilonRulesWidget(this);
            break;
        case DFA:
            m_centralWidget = new CFADeterminizationWidget(this);
            break;
        case CFG_TO_PDA:
            m_centralWidget = new CCfgToPdaWidget(this);
            break;
        case none:
        default:
            this->setCentralWidget(new QWidget());
            showStatusMessage(tr("ERROR: No conversion selected!"));
            break;
    }
    this->setCentralWidget(m_centralWidget);
    m_centralWidget->ConnectChangeMode(this, &MainWindow::modeChanged);
    m_centralWidget->ConnectStatusMessage(this, &MainWindow::showStatusMessage);
    emit modeChanged(m_mode);
}


/////////////////////////////////////////
/////////////////////////////////////////
// RegExp to FA EXAMPLES
/////////////////////////////////////////
/////////////////////////////////////////

void MainWindow::RE_FA_example(RegExp *_re, QString example_name)
{
    on_action_check_mode_triggered();
    ui->action_RE_to_FA->setChecked(true);
    if(m_activeConversion != RE_to_FA)
        PrepareConversionWidget(Conversions::RE_to_FA);
    ((CRegExpToFaWidget*)m_centralWidget)->SetInputRegExp(_re);
	m_exampleName = example_name;
	mySetWindowTitle();

}

void MainWindow::on_RE_FA_example0_triggered()
{
    RE_FA_example(new RegExp(), tr("Example 1"));
}

void MainWindow::on_RE_FA_example1_triggered()
{
    RE_FA_example(new RegExp(EPSILON));
}

void MainWindow::on_RE_FA_example2_triggered()
{
    RE_FA_example(new RegExp("a"));
}

void MainWindow::on_RE_FA_example3_triggered()
{
    RE_FA_example(new RegExp("ab"));;
}

void MainWindow::on_RE_FA_example4_triggered()
{
    RE_FA_example(new RegExp("a*"));;
}

void MainWindow::on_RE_FA_example5_triggered()
{
    RE_FA_example(new RegExp("a+b"));;
}

void MainWindow::on_RE_FA_example6_triggered()
{
    RE_FA_example(new RegExp("a+b*"));;
}

void MainWindow::on_RE_FA_example7_triggered()
{
    RE_FA_example(new RegExp("(a+b)*"));;
}

void MainWindow::on_RE_FA_example8_triggered()
{
    RE_FA_example(new RegExp("((a+b)*+c)*"));;
}

void MainWindow::on_RE_FA_example9_triggered()
{
    RE_FA_example(new RegExp("(a+b)*(a+c)*"));;
}

void MainWindow::RemoveEpsilon_example(FiniteAutomata _FA, QString example_name)
{
    on_action_check_mode_triggered();
    ui->action_RemoveEpsilon->setChecked(true);

    if(m_activeConversion != REMOVE_EPSILON)
        PrepareConversionWidget(REMOVE_EPSILON);
    ((CRemoveEpsilonRulesWidget*)m_centralWidget)->SetInputFA(_FA);
	m_exampleName = example_name;
	mySetWindowTitle();
}

void MainWindow::on_RemoveEpsilon_example0_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1" << "2" << "3";
    FA.m_startState = "0";
    FA.m_finalStates << "2" << "3";
    FA.m_alphabet << "a" << "b";
    FA.m_rules << ComputationalRules("0","1",EPSILON) << ComputationalRules("1","2","a") << ComputationalRules("1","3","b");
    FA.m_coordinates["0"] = QPoint(165,279);
    FA.m_coordinates["1"] = QPoint(259,279);
    FA.m_coordinates["2"] = QPoint(344,336);
    FA.m_coordinates["3"] = QPoint(350,223);
    RemoveEpsilon_example(FA, tr("Example 1"));
}

void MainWindow::on_RemoveEpsilon_example1_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "s" << "q" << "f";
    FA.m_startState = "s";
    FA.m_finalStates << "f";
    FA.m_alphabet << "0" << "1";
    FA.m_rules << ComputationalRules("s","q","1") << ComputationalRules("q","s","0")
             << ComputationalRules("q","f",EPSILON) << ComputationalRules("f","q",EPSILON)
             << ComputationalRules("f","s","1") << ComputationalRules("s","f","0")   ;
    FA.m_coordinates["s"] = QPoint(166,292);
    FA.m_coordinates["q"] = QPoint(266,81);
    FA.m_coordinates["f"] = QPoint(389,295);
    RemoveEpsilon_example(FA, tr("Example 2"));
}

void MainWindow::on_RemoveEpsilon_example2_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "s" << "f1" << "f2" << "q1" << "q2" << "q3";
    FA.m_startState = "s";
    FA.m_finalStates << "f1" << "f2";
    FA.m_alphabet << "a" << "b";
    FA.m_rules << ComputationalRules("s","f1",EPSILON) << ComputationalRules("s","f2",EPSILON)
             << ComputationalRules("f1","q1","a") << ComputationalRules("q1","f1","b")
             << ComputationalRules("f2","q2","a") << ComputationalRules("q2","q3","b")
             << ComputationalRules("q3","f2","a");
    FA.m_coordinates["s"]  = QPoint(176,196);
    FA.m_coordinates["f1"] = QPoint(175,113);
    FA.m_coordinates["f2"] = QPoint(178,293);
    FA.m_coordinates["q1"] = QPoint(357,114);
    FA.m_coordinates["q2"] = QPoint(282,189);
    FA.m_coordinates["q3"] = QPoint(338,295);
    RemoveEpsilon_example(FA, tr("Example 3"));
}

void MainWindow::on_RemoveEpsilon_example3_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "s" << "q1" << "q2" << "f";
    FA.m_startState = "s";
    FA.m_finalStates << "f";
    FA.m_alphabet << "a" << "b";
    FA.m_rules
        << ComputationalRules("s","q2","a")
        << ComputationalRules("q1","s","a")
        << ComputationalRules("q1","q2","b")
        << ComputationalRules("q2","f",EPSILON)
        << ComputationalRules("q1","f","b")
        << ComputationalRules("f","q1",EPSILON)
        << ComputationalRules("q1","q2","b");
    FA.m_coordinates["s"] = QPoint(168,192);
    FA.m_coordinates["q1"] = QPoint(265,89);
    FA.m_coordinates["q2"] = QPoint(271,295);
    FA.m_coordinates["f"] = QPoint(386,202);
    RemoveEpsilon_example(FA, tr("Example 4"));
}

void MainWindow::on_RemoveEpsilon_advanced_example1_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "s" << "q1" << "q2" << "f";
    FA.m_startState = "s";
    FA.m_finalStates << "f";
    FA.m_alphabet << "a" << "b" <<"c";
    FA.m_rules
        << ComputationalRules("s","q1",EPSILON)
        << ComputationalRules("s","q2",EPSILON)
        << ComputationalRules("q1","f","b")
        << ComputationalRules("q2","f","c")
        << ComputationalRules("s","s","a")
        << ComputationalRules("q1","q1","b")
        << ComputationalRules("q2","q2","c")
        << ComputationalRules("f","f","a");
    FA.m_coordinates["s"] = QPoint(169,287);
    FA.m_coordinates["q1"] = QPoint(280,220);
    FA.m_coordinates["q2"] = QPoint(279,346);
    FA.m_coordinates["f"] = QPoint(395,291);
    RemoveEpsilon_example(FA, tr("Advanced example 1"));
}

void MainWindow::on_RemoveEpsilon_advanced_example2_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1" << "2" << "3" << "4";
    FA.m_startState = "0";
    FA.m_finalStates << "2";
    FA.m_alphabet << "a" << "b";
    FA.m_rules
        << ComputationalRules("0","1",EPSILON)
        << ComputationalRules("0","3",EPSILON)
        << ComputationalRules("1","2",EPSILON)
        << ComputationalRules("3","2",EPSILON)
        << ComputationalRules("1","1","a")
        << ComputationalRules("2","2","b")
        << ComputationalRules("3","4","a")
        << ComputationalRules("4","3","b");
    FA.m_coordinates["0"] = QPoint(168,194);
    FA.m_coordinates["1"] = QPoint(235,114);
    FA.m_coordinates["2"] = QPoint(319,192);
    FA.m_coordinates["3"] = QPoint(230,288);
    FA.m_coordinates["4"] = QPoint(394,294);
    RemoveEpsilon_example(FA, tr("Advanced example 2"));
}

void MainWindow::on_RemoveEpsilon_advanced_example3_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1" << "2" << "3";
    FA.m_startState = "0";
    FA.m_finalStates << "3";
    FA.m_alphabet << "a" << "b" << "c" << "d";
    FA.m_rules
        << ComputationalRules("0","0",EPSILON)
        << ComputationalRules("0","0","a")
        << ComputationalRules("3","0","a")
        << ComputationalRules("2","0",EPSILON)
        << ComputationalRules("0","1",EPSILON)
        << ComputationalRules("1","1","c")
        << ComputationalRules("1","3","a")
        << ComputationalRules("3","3","b")
        << ComputationalRules("3","2",EPSILON)
        << ComputationalRules("2","2","d");
    FA.m_coordinates["0"] = QPoint(153,177);
    FA.m_coordinates["1"] = QPoint(264,109);
    FA.m_coordinates["2"] = QPoint(273,283);
    FA.m_coordinates["3"] = QPoint(386,179);
    RemoveEpsilon_example(FA, tr("Advanced example 3"));
}

void MainWindow::on_RemoveEpsilon_advanced_example4_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1" << "2" << "3" << "4" << "5";
    FA.m_startState = "0";
    FA.m_finalStates << "5";
    FA.m_alphabet << "a" << "b" << "c" << "d";
    FA.m_rules
        << ComputationalRules("0","5",EPSILON)
        << ComputationalRules("1","0",EPSILON)
        << ComputationalRules("2","0",EPSILON)
        << ComputationalRules("3","1",EPSILON)
        << ComputationalRules("4","2",EPSILON)
        << ComputationalRules("3","3","b")
        << ComputationalRules("4","4","a")
        << ComputationalRules("5","4","c")
        << ComputationalRules("3","5",EPSILON)
        << ComputationalRules("5","3","d");
    FA.m_coordinates["0"] = QPoint(163,190);
    FA.m_coordinates["1"] = QPoint(207,108);
    FA.m_coordinates["2"] = QPoint(209,297);
    FA.m_coordinates["3"] = QPoint(296,114);
    FA.m_coordinates["4"] = QPoint(337,296);
    FA.m_coordinates["5"] = QPoint(382,199);
    RemoveEpsilon_example(FA, tr("Advanced example 4"));
}

void MainWindow::Determinization_example(FiniteAutomata _FA, QString example_name)
{
    on_action_check_mode_triggered();
    ui->action_Determinization->setChecked(true);

    if(m_activeConversion != DFA)
        PrepareConversionWidget(Conversions::DFA);
    ((CFADeterminizationWidget*)m_centralWidget)->SetInputFA(_FA);
	m_exampleName = example_name;
	mySetWindowTitle();
}

void MainWindow::on_Determinization_example_1_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1";
    FA.m_startState = "0";
    FA.m_finalStates << "1";
    FA.m_alphabet << "a" << "b";
    FA.m_rules
        << ComputationalRules("0","0","a")
        << ComputationalRules("0","0","b")
        << ComputationalRules("0","1","b");
    FA.m_coordinates["0"] = QPoint(178,227);
    FA.m_coordinates["1"] = QPoint(316,230);
    Determinization_example(FA, tr("Example 1"));
}
void MainWindow::on_Determinization_example_2_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "A" << "B";
    FA.m_startState = "A";
    FA.m_finalStates << "B";
    FA.m_alphabet << "0" << "1";
    FA.m_rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","A","1")
        << ComputationalRules("B","B","0")
        << ComputationalRules("A","B","0");
    FA.m_coordinates["A"] = QPoint(173,198);
    FA.m_coordinates["B"] =  QPoint(318,200);
    Determinization_example(FA, tr("Example 2"));
}

void MainWindow::on_Determinization_example_3_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "0" << "1" << "2" << "3";
    FA.m_startState = "0";
    FA.m_finalStates << "3";
    FA.m_alphabet << "a" << "b";
    FA.m_rules
        << ComputationalRules("0","0","b")
        << ComputationalRules("0","1","b")
        << ComputationalRules("3","1","b")
        << ComputationalRules("3","1","a")
        << ComputationalRules("2","3","b")
        << ComputationalRules("1","2","b")
        << ComputationalRules("2","1","a");
    FA.m_coordinates["0"] = QPoint(184,149);
    FA.m_coordinates["1"] = QPoint(188,249);
    FA.m_coordinates["2"] = QPoint(186,417);
    FA.m_coordinates["3"] = QPoint(385,249);
    Determinization_example(FA, tr("Example 3"));
}

void MainWindow::on_Determinization_example_4_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "s" << "q1" << "q2" << "f";
    FA.m_startState = "s";
    FA.m_finalStates << "f";
    FA.m_alphabet << "a" << "b" <<"c";
    FA.m_rules
        << ComputationalRules("s","s","a")
        << ComputationalRules("q1","q1","b")
        << ComputationalRules("q2","q2","c")
        << ComputationalRules("f","f","a")
        << ComputationalRules("s","q1","b")
        << ComputationalRules("s","f","b")
        << ComputationalRules("s","f","c")
        << ComputationalRules("s","q2","c")
        << ComputationalRules("q1","f","b")
        << ComputationalRules("q2","f","c");
    FA.m_coordinates["s"] = QPoint(169,315);
    FA.m_coordinates["q1"] = QPoint(279,425);
    FA.m_coordinates["q2"] = QPoint(277,231);
    FA.m_coordinates["f"] = QPoint(396,315);
    Determinization_example(FA, tr("Example 4"));
}


void MainWindow::on_Determinization_advanced_example_1_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "A" << "B" << "C" << "D";
    FA.m_startState = "A";
    FA.m_finalStates << "D";
    FA.m_alphabet << "0" << "1";
    FA.m_rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","A","1")
        << ComputationalRules("A","B","1")
        << ComputationalRules("B","C","0")
        << ComputationalRules("B","C","1")
        << ComputationalRules("C","D","0")
        << ComputationalRules("C","D","1");
    FA.m_coordinates["A"] = QPoint(161,279);
    FA.m_coordinates["B"] = QPoint(246,281);
    FA.m_coordinates["C"] = QPoint(333,282);
    FA.m_coordinates["D"] = QPoint(410,282);
    Determinization_example(FA, tr("Advanced example 1"));
}

void MainWindow::on_Determinization_advanced_example_2_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "A" << "B" << "C";
    FA.m_startState = "A";
    FA.m_finalStates << "B";
    FA.m_alphabet << "0" << "1";
    FA.m_rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","B","1")
        << ComputationalRules("B","A","0")
        << ComputationalRules("C","A","1")
        << ComputationalRules("B","C","0")
        << ComputationalRules("C","B","1");
    FA.m_coordinates["A"] = QPoint(178,201);
    FA.m_coordinates["B"] = QPoint(278,402);
    FA.m_coordinates["C"] = QPoint(378,198);
    Determinization_example(FA, tr("Advanced example 2"));
}

void MainWindow::on_Determinization_advanced_example_3_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "A" << "B" << "C" << "D" << "E";
    FA.m_startState = "A";
    FA.m_finalStates << "D" << "E";
    FA.m_alphabet << "0" << "1";
    FA.m_rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","A","1")
        << ComputationalRules("D","D","0")
        << ComputationalRules("D","D","1")
        << ComputationalRules("E","E","0")
        << ComputationalRules("E","E","1")
        << ComputationalRules("A","C","0")
        << ComputationalRules("A","B","1")
        << ComputationalRules("C","E","0")
        << ComputationalRules("B","D","1");
    FA.m_coordinates["A"] = QPoint(167,135);
    FA.m_coordinates["B"] = QPoint(167,365);
    FA.m_coordinates["C"] = QPoint(254,244);
    FA.m_coordinates["D"] = QPoint(286,367);
    FA.m_coordinates["E"] = QPoint(370,249);
    Determinization_example(FA, tr("Advanced example 3"));
}

void MainWindow::on_Determinization_advanced_example_4_triggered()
{
    FiniteAutomata FA;
    FA.m_states << "A" << "B" << "C";
    FA.m_startState = "A";
    FA.m_finalStates << "C";
    FA.m_alphabet << "0" << "1";
    FA.m_rules
        << ComputationalRules("A","A","1")
        << ComputationalRules("A","C","0")
        << ComputationalRules("A","C","1")
        << ComputationalRules("C","A","1")
        << ComputationalRules("C","B","0")
        << ComputationalRules("B","B","1")
        << ComputationalRules("B","A","0")
        << ComputationalRules("B","A","1");
    FA.m_coordinates["A"] = QPoint(159,266);
    FA.m_coordinates["B"] = QPoint(309,152);
    FA.m_coordinates["C"] = QPoint(326,363);
    Determinization_example(FA, tr("Advanced example 4"));
}

void MainWindow::CfgToPda_example(const CContextFreeGrammar& cfg, QString example_name)
{
    //on_action_check_mode_triggered();
    ui->m_actionCFGtoPDA->setChecked(true);

    if(m_activeConversion != CFG_TO_PDA)
        PrepareConversionWidget(Conversions::CFG_TO_PDA);
    ((CCfgToPdaWidget*)m_centralWidget)->SetCfg(cfg);

    /* Abylity to set PDA to pda widget
    CPushDownAutomata pda;
    pda.SetStartState("s");
    pda.SetStates(QSet<QString>{"s","s1","s2","f1","f2"});
    pda.SetFinalStates(QSet<QString>{"f1","f2"});
    pda.SetAplhabet(QSet<QString>{"a","b","c"});
    pda.SetStackAlphabet(QSet<QString>{"D","E","F"});
    pda.AddPDARule("s","s","a","D",QVector<QString>{"E","F"});
    pda.AddPDARule("s","s1","b","E",QVector<QString>{"D","F"});
    pda.AddPDARule("s2","f1","c","F","D");
    ((CCfgToPdaWidget*)m_centralWidget)->SetPda(pda);
    */
	m_exampleName = example_name;
    mySetWindowTitle();
}

void MainWindow::on_CfgToPda_Example_1_triggered()
{
    // Initialize context free grammer
    CContextFreeGrammar g;
    CNonTerminal S("S");
    g.SetStartNonTerminal(S);
    QSet<CNonTerminal> nonterminals;
    nonterminals.insert(S);
    g.SetNonterminalsAlphabet(nonterminals);
    QSet<CTerminal> terminals;
    CTerminal leftBracket("{");
    CTerminal rightBracket("}");
    terminals.insert(leftBracket);
    terminals.insert(rightBracket);
    g.SetTerminalsAlphabet(terminals);
    QSet<CCFGRule> rules;
    QVector<CSymbol> rightRulePart1= {leftBracket, S, rightBracket};
    QVector<CSymbol> rightRulePart2 = {leftBracket, rightBracket};
    rules.insert(CCFGRule(S, rightRulePart1));
    rules.insert(CCFGRule(S, rightRulePart2));
    g.SetRules(rules);
    CfgToPda_example(g,tr("Simple example 1"));
}

void MainWindow::on_action_save_triggered()
{
    if(m_activeConversion == none)
    {
        showStatusMessage(tr("ERROR: No conversion selected!"));
        return;
    }
    QString filename = "";
    if((filename = QFileDialog::getSaveFileName(this,tr("Save conversion"))) == "")
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << m_activeConversion << m_mode;
    switch(m_activeConversion)
    {
        case RE_to_FA:
        {
            out << ((CRegExpToFaWidget*)m_centralWidget)->GetInputRegExp()->m_regExpStr;
        }
        break;
        case REMOVE_EPSILON:
            out << ((CFADeterminizationWidget*)m_centralWidget)->GetInputFA();
            // In play mode does not make sence to save output fa
            if(m_mode != AlgorithmModes::algorithmSteping)
                out << ((CFADeterminizationWidget*)m_centralWidget)->GetOutputFA();
            break;
        case DFA:
            out << ((CFADeterminizationWidget*)m_centralWidget)->GetInputFA();
            if(m_mode != AlgorithmModes::algorithmSteping)
            {
                out <<  ((CFADeterminizationWidget*)m_centralWidget)->GetOutputFA();
            }
            break;
        // This should never happend
        case none:
            showStatusMessage(tr("ERROR: No conversion selected!"));
            break;
    }
    file.close();
}

void MainWindow::on_action_open_file_triggered()
{
    QString filename = "";
    if((filename = QFileDialog::getOpenFileName(this, tr("Open conversion")))=="")
        return;

    QFile read(filename);
    read.open(QIODevice::ReadOnly);
    QDataStream in(&read);    // read the data serialized from the file
    FiniteAutomata FA;
    //
    Conversions conversion = none;
    AlgorithmModes tmp_mode = AlgorithmModes::NONE;

    in >> conversion;
    in >> tmp_mode;
    switch (tmp_mode)
    {
        case AlgorithmModes::individualWork:
            on_action_check_mode_triggered();
        break;
        case AlgorithmModes::algorithmSteping:
            on_action_play_mode_triggered();
        break;
        case AlgorithmModes::instantChecking:
            on_action_step_mode_triggered();
        break;
        case AlgorithmModes::NONE:
            showStatusMessage(tr("ERROR: Input file in wrong format!"));
            return;
        break;
    }
    m_mode = tmp_mode;

    // Check that m_centralWidget was created and that if was created for actual conversion
    if(!m_centralWidget || m_activeConversion != conversion)
    {
        PrepareConversionWidget(conversion);
    }

    switch(conversion)
    {
        case RE_to_FA:
        {
            QString regexp_str;
            in >> regexp_str;
            ((CRegExpToFaWidget*)m_centralWidget)->SetInputRegExp(new RegExp(regexp_str));
            break;
        }
        case REMOVE_EPSILON:
        {
            FiniteAutomata in_FA;
            in >> in_FA;
            RemoveEpsilon_example(in_FA);
            if(m_mode != AlgorithmModes::algorithmSteping)
            {
                FiniteAutomata out_FA;
                in >> out_FA;
                ((CRemoveEpsilonRulesWidget*)m_centralWidget)->SetOutputFA(out_FA);
            }
        }
        break;
        case DFA:
        {
            FiniteAutomata in_FA;
            in >> in_FA;
            Determinization_example(in_FA);
            if(m_mode != AlgorithmModes::algorithmSteping)
            {
                FiniteAutomata outputFA;
                in >> outputFA;
                ((CFADeterminizationWidget*)m_centralWidget)->SetOutputFA(outputFA);
            }
        }
        break;
        case none:
            showStatusMessage(tr("ERROR: Input file in wrong format!"));
            return;
        break;
    }
}

QDataStream& operator>>(QDataStream& in, MainWindow::Conversions& e)
{
    quint32 tmp;
    in >> tmp;
    e = (MainWindow::Conversions)tmp;
    return in;
}



void MainWindow::on_actionCzech_triggered()
{
    translator->load(":/translations/language/RegularConvertor_cs_CZ.qm");
    qApp->installTranslator(translator);
    ui->retranslateUi(this);
}

void MainWindow::on_actionEnglish_triggered()
{
    qApp->removeTranslator(translator);
}


void MainWindow::SetActionsGroups()
{

    QActionGroup* AlgorithmModesGroup = new QActionGroup(this);
    AlgorithmModesGroup->addAction(ui->action_check_mode);
    AlgorithmModesGroup->addAction(ui->action_play_mode);
    AlgorithmModesGroup->addAction(ui->action_step_mode);

    QActionGroup* conversionGroup = new QActionGroup(this);
    conversionGroup->addAction(ui->action_RE_to_FA);
    conversionGroup->addAction(ui->action_RemoveEpsilon);
    conversionGroup->addAction(ui->action_Determinization);

    QActionGroup* examples_group = new QActionGroup(this);
    examples_group->addAction(ui->RE_FA_example0);
    examples_group->addAction(ui->RE_FA_example1);
    examples_group->addAction(ui->RE_FA_example2);
    examples_group->addAction(ui->RE_FA_example3);
    examples_group->addAction(ui->RE_FA_example4);
    examples_group->addAction(ui->RE_FA_example5);
    examples_group->addAction(ui->RE_FA_example6);
    examples_group->addAction(ui->RE_FA_example7);
    examples_group->addAction(ui->RE_FA_example8);
    examples_group->addAction(ui->RE_FA_example9);
    examples_group->addAction(ui->RemoveEpsilon_example0);
    examples_group->addAction(ui->RemoveEpsilon_example1);
    examples_group->addAction(ui->RemoveEpsilon_example2);
    examples_group->addAction(ui->RemoveEpsilon_example3);
    examples_group->addAction(ui->RemoveEpsilon_advanced_example1);
    examples_group->addAction(ui->RemoveEpsilon_advanced_example2);
    examples_group->addAction(ui->RemoveEpsilon_advanced_example3);
    examples_group->addAction(ui->RemoveEpsilon_advanced_example4);
    examples_group->addAction(ui->Determinization_example_1);
    examples_group->addAction(ui->Determinization_example_2);
    examples_group->addAction(ui->Determinization_example_3);
    examples_group->addAction(ui->Determinization_example_4);
    examples_group->addAction(ui->Determinization_advanced_example_1);
    examples_group->addAction(ui->Determinization_advanced_example_2);
    examples_group->addAction(ui->Determinization_advanced_example_3);
    examples_group->addAction(ui->Determinization_advanced_example_4);

    QActionGroup* language_group = new QActionGroup(this);
    language_group->addAction(ui->actionCzech);
    language_group->addAction(ui->actionEnglish);
}

void MainWindow::on_action_Determinization_triggered()
{
	m_exampleName = "";
	mySetWindowTitle();
	PrepareConversionWidget(MainWindow::Conversions::DFA);
}

void MainWindow::on_action_RE_to_FA_triggered()
{
	m_exampleName = "";
	mySetWindowTitle();
    PrepareConversionWidget(MainWindow::Conversions::RE_to_FA);
}

void MainWindow::on_action_RemoveEpsilon_triggered()
{
	m_exampleName = "";
	mySetWindowTitle();
	PrepareConversionWidget(MainWindow::Conversions::REMOVE_EPSILON);
}

void MainWindow::on_actionCFGtoPDA_triggered()
{
	m_exampleName = "";
	mySetWindowTitle();
    PrepareConversionWidget(CFG_TO_PDA);
}
