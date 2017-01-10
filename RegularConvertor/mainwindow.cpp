#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "algorithms/htmldelegate.h"
#include "algorithms/algorithmview.h"
#include "algorithms/algorithmwidget.h"
#include <widgets/cfgtopdawidget.h>

#define MY_APP_NAME "Regular Convertor"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_centralWidget(NULL)
{

    ui->setupUi(this);
    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);
    //this->ui->actionSimple_example_1->

    statusBarTimeout = 5000; //5 second
    setWindowTitle(MY_APP_NAME);
    connect(ui->action_RE_to_FA,SIGNAL(triggered()),this,SLOT(prepareREtoFA()));
    connect(ui->action_RemoveEpsilon,SIGNAL(triggered()),this,SLOT(prepareRemoveEpsilon()));
    connect(ui->action_Determinization,SIGNAL(triggered()),this,SLOT(PrepareDFA()));

    SetActionsGroups();

    reg_exp_algorithm = 0;
    remove_epsilon_algorithm = 0;
    m_activeConversion = none;
    mode = Algorithm::PLAY_MODE;
    ui->action_play_mode->setChecked(true);

    alhgorithm_widget = new CAlgorithmWidget(mode);
    alhgorithm_widget->hide();
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), this, SLOT(mySetWindowTitle()));
    //    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
    //    connect( deleteShortCut, SIGNAL(activated()), FA1_widget->scene, SLOT(deleteSelected()));


    // Custom status bar
    status_label = new QLabel(this);
    statusBar()->addWidget(status_label);
    status_label->hide();
    status_timer = new QTimer(this);
    status_timer->setInterval(5000);

    status_label->setStyleSheet("QLabel { background-color : white;}");

    translator = new QTranslator(this);

    connect(status_timer,SIGNAL(timeout()),this,SLOT(hideStatusMessage()));

//    QIcon my_icon = QIcon(":/main_window/pictures/Flag_of_the_Czech_Republic.png");
//    QToolButton* language_button = new QToolButton();
//    language_button->setIcon(my_icon);
//    language_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
//    language_button->setIconSize(QSize(100,100));
//    QHBoxLayout* hlayout = new QHBoxLayout(ui->menuBar);
//    hlayout->addStretch();
//    hlayout->addWidget(language_button);

    ui->menuLanguages->menuAction()->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mySetWindowTitle(QString example_name)
{
    QString conversion_str;
    QString mode_str;
    switch(m_activeConversion)
    {
        case none:
            conversion_str="";
        break;
        case RE_to_FA:
            conversion_str=tr("RegExp to Finite autoamta");
        break;
        case REMOVE_EPSILON:
            conversion_str=tr("Remove epsilon rules");
        break;
        case DFA:
            conversion_str=tr("FA determinization");
        break;
    }

    switch(mode)
    {
        case Algorithm::NONE:
            mode_str = "";
        break;
        case Algorithm::CHECK_MODE:
            mode_str = tr("Mode: individual work");
        break;
        case Algorithm::PLAY_MODE:
            mode_str = tr("Mode: algorithm stepping");
        break;
        case Algorithm::STEP_MODE:
            mode_str = tr("Mode: instant checking");
        break;
    }
    QString example_str;
    if(example_name != "")
        example_str = " - " + example_name;
    setWindowTitle(MY_APP_NAME " - " + conversion_str + " - " + mode_str + example_str);

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

QWidget *MainWindow::prepareAlgorithnContainer(QWidget *central_w, QString str_label, Algorithm *algorithm)
{
    QWidget* algorithm_container = new QWidget(central_w);
    QVBoxLayout* algorithm_vlayout = new QVBoxLayout(algorithm_container);
    algorithm_vlayout->setMargin(0);
    QLabel* algorithm_label = new QLabel(str_label,central_w);
    algorithm_label->setAlignment(Qt::AlignCenter);
    alhgorithm_widget->show();
    algorithm_vlayout->addWidget(algorithm_label);
    algorithm_vlayout->addWidget(alhgorithm_widget);
    HTMLDelegate* delegate = new HTMLDelegate();
    alhgorithm_widget->getAlgorithmView()->setModel(algorithm);
    alhgorithm_widget->getAlgorithmView()->setItemDelegate(delegate);
    connect(delegate,SIGNAL(dataChanged(QModelIndex)),algorithm,SLOT(getData(QModelIndex)));
    connect(this,SIGNAL(modeChanged(Algorithm::modes)),algorithm,SLOT(setMode(Algorithm::modes)));
    return algorithm_container;
}

QWidget *MainWindow::prepareFAContainer(QWidget *central_w, QString str_label, FA_widget * fa_widget)
{
    QWidget* fa_container = new QWidget(central_w);
    QVBoxLayout* FA_left_vlayout = new QVBoxLayout(fa_container);
    FA_left_vlayout->setMargin(0);
    QLabel* FA_left_label = new QLabel(str_label,central_w);
    FA_left_label->setAlignment(Qt::AlignCenter);
    FA_left_vlayout->addWidget(FA_left_label);
    FA_left_vlayout->addWidget(fa_widget);
    return fa_container;
}

QWidget *MainWindow::variablesContainer(QWidget* central_w, QString str_label, QLabel *var_widget)
{
    QWidget* variables_container = new QWidget(central_w);
    QVBoxLayout* variables_vlayout = new QVBoxLayout(variables_container);
    QLabel* variables_label = new QLabel(str_label,central_w);
    variables_vlayout->addWidget(variables_label);
    var_widget->setStyleSheet("QLabel { background-color : white; color : black; }");
    variables_vlayout->addWidget(var_widget);
    var_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    return variables_container;
}

QWidget *MainWindow::horizontalContainer(QWidget* central_w, QList<QWidget *> widgets)
{
    QWidget* container = new QWidget(central_w);
    QHBoxLayout* horizontal_layout1 = new QHBoxLayout(container);
    horizontal_layout1->setMargin(0);
    QSplitter* h_spitter = new QSplitter(Qt::Horizontal,container);
    container->layout()->addWidget(h_spitter);
    foreach(QWidget* w, widgets)
    {
        h_spitter->addWidget(w);
    }
    return container;
}

QWidget *MainWindow::verticalContainer(QWidget* central_w, QList<QWidget *> widgets)
{
    QWidget* container = new QWidget(central_w);
    QVBoxLayout* vertical_layout1 = new QVBoxLayout(container);
    vertical_layout1->setMargin(0);
    QSplitter* v_spitter = new QSplitter(Qt::Vertical,container);
    container->layout()->addWidget(v_spitter);
    foreach(QWidget* w, widgets)
    {
        v_spitter->addWidget(w);
    }
    return container;
}

void MainWindow::prepareREtoFA(RegExp* _re)
{
//    if(activeConversion != RE_to_FA)
//    {
        m_activeConversion = RE_to_FA;

        //basic components
        regExpToFA_central_widget = new QWidget(this);
        fa_widget_left = new FA_widget(regExpToFA_central_widget);
        fa_widget_center = new FA_widget(regExpToFA_central_widget);
        fa_widget_right = new FA_widget(regExpToFA_central_widget);
        connect(fa_widget_left,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_widget_center,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_widget_right,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        reg_exp_widget = new RegExpWidget(regExpToFA_central_widget);
        alhgorithm_widget = new CAlgorithmWidget(mode,regExpToFA_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
        reg_exp_algorithm = new RegExpToFA(alhgorithm_widget, mode, reg_exp_widget, fa_widget_left, fa_widget_center, fa_widget_right, _re, regExpToFA_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), reg_exp_algorithm, SLOT(setMode(Algorithm::modes)));
        prepareREtoFA_GUI();
//    }
//    else
//    {
//        ;
//    }
}

void MainWindow::prepareREtoFA_GUI()
{
    ui->textBrowser = 0;

    mySetWindowTitle();

    //set central widget
    QWidget* w = regExpToFA_central_widget;
    this->setCentralWidget(w);
    QLayout* layout = new QGridLayout(w);
    layout->setMargin(0);


    //spitters
    QSplitter* h_spitter1 = new QSplitter(Qt::Horizontal,w);
    QSplitter* h_spitter2 = new QSplitter(Qt::Horizontal,w);
    QSplitter* v_spitter1 = new QSplitter(Qt::Vertical,w);

    //regular expression
    QWidget* reg_exp_container = new QWidget(w);
    QVBoxLayout* reg_exp_vlayout = new QVBoxLayout(reg_exp_container);
    reg_exp_vlayout->setMargin(0);
    reg_exp_vlayout->addWidget(reg_exp_widget);

    //algorithm
    QWidget* algorithm_container = prepareAlgorithnContainer(w, tr("<b>Algorithm RegExp to FA</b>"), reg_exp_algorithm);

    //left FA
    QWidget* left_fa_container = prepareFAContainer(w, tr("<b>left son</b>"), fa_widget_left);

    //center FA
    QWidget* center_fa_container = prepareFAContainer(w, tr("<b>chosen node</b>"), fa_widget_center);

    //right FA
    QWidget* right_fa_container = prepareFAContainer(w, tr("<b>right son</b>"), fa_widget_right);


    //top container
    QWidget* up_container = new QWidget(w);
    QHBoxLayout* horizontal_layout1 = new QHBoxLayout(up_container);
    horizontal_layout1->setMargin(0);
    up_container->layout()->addWidget(h_spitter1);
    h_spitter1->addWidget(reg_exp_container);
    h_spitter1->addWidget(algorithm_container);

    //down container
    QWidget* down_container = new QWidget(w);
    h_spitter2->addWidget(left_fa_container);
    h_spitter2->addWidget(center_fa_container);
    h_spitter2->addWidget(right_fa_container);
    QHBoxLayout* horizontal_layout2 = new QHBoxLayout(down_container);
    horizontal_layout2->setMargin(0);
    horizontal_layout2->setSpacing(0);
    down_container->layout()->addWidget(h_spitter2);

    //vertical splitter
    v_spitter1->addWidget(up_container);
    v_spitter1->addWidget(down_container);
    w->layout()->addWidget(v_spitter1);
}



void MainWindow::prepareRemoveEpsilon()
{
    //if(activeConversion != REMOVE_EPSILON)
    //{
        m_activeConversion = REMOVE_EPSILON;

        //basic components
        removeEpsilon_central_widget = new QWidget(this);
        alhgorithm_widget = new CAlgorithmWidget(mode,removeEpsilon_central_widget);
        fa_epsilon_widget = new FA_widget(removeEpsilon_central_widget);
        fa_not_epsilon_widget = new FA_widget(removeEpsilon_central_widget);
        connect(fa_epsilon_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_not_epsilon_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        remove_epsilon_variables_widget = new QLabel(removeEpsilon_central_widget);
        epsilon_closer_list_widget = new QListWidget(removeEpsilon_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
        remove_epsilon_algorithm = new RemoveEpsilon(mode, alhgorithm_widget, fa_epsilon_widget, fa_not_epsilon_widget, remove_epsilon_variables_widget, epsilon_closer_list_widget, removeEpsilon_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), remove_epsilon_algorithm, SLOT(setMode(Algorithm::modes)));
        prepareRemoveEpsilon_GUI();
    //}
    //else
    //{
    //   ;
    //}
}



void MainWindow::prepareRemoveEpsilon_GUI()
{
    mySetWindowTitle();
     delete this->centralWidget();

    //set central widget
    QWidget* w = removeEpsilon_central_widget;
    this->setCentralWidget(w);
    QLayout* layout = new QGridLayout(w);
    layout->setMargin(0);

    //algorithm container
    QWidget* algorithm_container = prepareAlgorithnContainer(w, tr("<b>Algorithm: Remove epsilon rules</b>"), remove_epsilon_algorithm);

    //input FA container
    QWidget* fa_epsilon_container = prepareFAContainer(w, tr("<b>input FA</b>"), fa_epsilon_widget);

    //output FA container
    QWidget* fa_not_epsilon_container = prepareFAContainer(w, "<b>output FA</b>", fa_not_epsilon_widget);

    //variables container
    QWidget* variables_container = variablesContainer(w, "<b>variables</b>", remove_epsilon_variables_widget);


    //top container
    QWidget* up_container = new QWidget(w);
    QHBoxLayout* horizontal_layout1 = new QHBoxLayout(up_container);
    horizontal_layout1->setMargin(0);
    up_container->setLayout(horizontal_layout1);
    QSplitter* h_spitter1 = new QSplitter(Qt::Horizontal,w);
    up_container->layout()->addWidget(h_spitter1);
    h_spitter1->addWidget(fa_epsilon_container);
    h_spitter1->addWidget(algorithm_container);

    //down right container
    QWidget* down_left_container = new QWidget(w);
    QSplitter* v_spitter3 = new QSplitter(Qt::Vertical,w);
    v_spitter3->addWidget(variables_container);
    v_spitter3->addWidget(epsilon_closer_list_widget);
    QHBoxLayout* horizontal_layout3 = new QHBoxLayout(down_left_container);
    horizontal_layout3->setMargin(0);
    horizontal_layout3->setSpacing(0);
    down_left_container->layout()->addWidget(v_spitter3);

    //down container
    QWidget* down_container = new QWidget(w);
    QSplitter* h_spitter2 = new QSplitter(Qt::Horizontal,w);
    h_spitter2->addWidget(fa_not_epsilon_container);
    h_spitter2->addWidget(down_left_container);
    QHBoxLayout* horizontal_layout2 = new QHBoxLayout(down_container);
    horizontal_layout2->setMargin(0);
    horizontal_layout2->setSpacing(0);
    down_container->layout()->addWidget(h_spitter2);

    //vertical splitter
    QSplitter* v_spitter1 = new QSplitter(Qt::Vertical,w);
    v_spitter1->addWidget(up_container);
    v_spitter1->addWidget(down_container);
    w->layout()->addWidget(v_spitter1);
    w->show();
}


void MainWindow::PrepareDFA()
{
    m_activeConversion = DFA;

    //basic components
    DFA_central_widget = new QWidget(this);
    alhgorithm_widget = new CAlgorithmWidget(mode,DFA_central_widget);   // TODO nevytvaret porad novy ale zmenit parrenta a vymazat
    not_DFA_widget = new FA_widget(DFA_central_widget);
    DFA_widget = new FA_widget(DFA_central_widget);
    connect(not_DFA_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
    connect(DFA_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
    DFA_variables_widget = new QLabel(DFA_central_widget);
    DFA_variables_widget->setStyleSheet("QLabel { background-color : white; color : black; }");
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
    DFA_algorithm = new FaToDFA(mode, alhgorithm_widget, not_DFA_widget, DFA_widget, DFA_variables_widget, DFA_central_widget);
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), DFA_algorithm, SLOT(setMode(Algorithm::modes)));
    connect(DFA_algorithm,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
    PrepareDFA_GUI();
}


void MainWindow::PrepareDFA_GUI()
{
    mySetWindowTitle();
    delete this->centralWidget();

    //set central widget
    QWidget* w = DFA_central_widget;
    this->setCentralWidget(w);
    QLayout* layout = new QGridLayout(w);
    layout->setMargin(0);

    //algorithm container
    QWidget* algorithm_container = prepareAlgorithnContainer(w, tr("<b>FA determinization</b>"), DFA_algorithm);

    //input FA container
    QWidget* in_fa_container = prepareFAContainer(w, tr("<b>input FA</b>"), not_DFA_widget);

    //output FA container
    QWidget* out_fa_container = prepareFAContainer(w, tr("<b>output FA</b>"), DFA_widget);

    //variables container
    QWidget* variables_container = variablesContainer(w, tr("<b>variables</b>"), DFA_variables_widget);
    QVBoxLayout* variables_layout = static_cast<QVBoxLayout*>(variables_container->layout());
    variables_layout->addStretch();

    //top container
    QWidget* up_container = horizontalContainer(w, QList<QWidget *>() << in_fa_container << algorithm_container);

    QWidget* downRightContainer = verticalContainer(w,QList<QWidget*>() << variables_container);

    //down container
    QWidget* down_container = horizontalContainer(w, QList<QWidget *>() << out_fa_container << downRightContainer);

    //vertical splitter
    QSplitter* v_spitter = new QSplitter(Qt::Vertical,w);
    v_spitter->addWidget(up_container);
    v_spitter->addWidget(down_container);
    w->layout()->addWidget(v_spitter);
    w->show();
}

void MainWindow::PrepareCFGtoPDA()
{
    m_activeConversion = CFG_TO_PDA;

    //basic components
    CFG_TO_PDA_central_widget = new QWidget(this);
	// TODO nevytvaret porad novy ale zmenit parrenta a vymazat

    alhgorithm_widget = new CAlgorithmWidget(mode,CFG_TO_PDA_central_widget);
    m_cfgWidget = new CCfgWidget(CFG_TO_PDA_central_widget);
    m_pdaWidget = new CPdaWidget(/*CFG_TO_PDA_central_widget*/);
    DFA_variables_widget = new QLabel(CFG_TO_PDA_central_widget);
    DFA_variables_widget->setStyleSheet("QLabel { background-color : white; color : black; }");
    CFG_TO_PDA_algorithm = new CCfgToPdaGuiInterface(/* mode, alhgorithm_widget, not_DFA_widget, DFA_widget, DFA_variables_widget, CFG_TO_PDA_central_widget*/);

    connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), CFG_TO_PDA_algorithm, SLOT(setMode(Algorithm::modes)));
    connect(CFG_TO_PDA_algorithm,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));

    prepareCFG_TO_PDA_GUI();

}

void MainWindow::prepareCFG_TO_PDA_GUI()
{
    this->setCentralWidget(new CCfgToPdaWidget(this));

    return;



  mySetWindowTitle();
  delete this->centralWidget();

  //set central widget
  QWidget* w = CFG_TO_PDA_central_widget;
  this->setCentralWidget(w);
  QLayout* layout = new QGridLayout(w);
  layout->setMargin(0);

  //algorithm container
  QWidget* algorithm_container = prepareAlgorithnContainer(w, tr("<b>CFG to PDA</b>"), CFG_TO_PDA_algorithm);

  //variables container
  QWidget* variables_container = variablesContainer(w, tr("<b>variables</b>"), DFA_variables_widget);
  QVBoxLayout* variables_layout = static_cast<QVBoxLayout*>(variables_container->layout());
  variables_layout->addStretch();

  //top container
  QWidget* up_container = horizontalContainer(w, QList<QWidget *>() << m_cfgWidget << algorithm_container);

  QWidget* downRightContainer = verticalContainer(w,QList<QWidget*>() << variables_container);

  //down container
  QWidget* down_container = horizontalContainer(w, QList<QWidget *>() << m_pdaWidget << downRightContainer);

  //vertical splitter
  QSplitter* v_spitter = new QSplitter(Qt::Vertical,w);
  v_spitter->addWidget(up_container);
  v_spitter->addWidget(down_container);
  w->layout()->addWidget(v_spitter);
  w->show();
}


void MainWindow::on_action_check_mode_triggered()
{
    mode = Algorithm::CHECK_MODE;
    ui->action_check_mode->setChecked(true);
    emit modeChanged(mode);
}

void MainWindow::on_action_play_mode_triggered()
{
    mode = Algorithm::PLAY_MODE;
    ui->action_play_mode->setChecked(true);
    emit modeChanged(mode);
}

void MainWindow::on_action_step_mode_triggered()
{
    mode = Algorithm::STEP_MODE;
    ui->action_step_mode->setChecked(true);
    emit modeChanged(mode);
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
            break;
        case REMOVE_EPSILON:
            break;
        case DFA:
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
        prepareREtoFA(_re);
    reg_exp_algorithm->setExample(_re);
    mySetWindowTitle(example_name);

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
        prepareRemoveEpsilon();
    remove_epsilon_algorithm->setInputFA(_FA);
    mySetWindowTitle(example_name);
}

void MainWindow::on_RemoveEpsilon_example0_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3";
    FA.startState = "0";
    FA.finalStates << "2" << "3";
    FA.alphabet << "a" << "b";
    FA.rules << ComputationalRules("0","1",EPSILON) << ComputationalRules("1","2","a") << ComputationalRules("1","3","b");
    FA.coordinates["0"] = QPoint(165,279);
    FA.coordinates["1"] = QPoint(259,279);
    FA.coordinates["2"] = QPoint(344,336);
    FA.coordinates["3"] = QPoint(350,223);
    RemoveEpsilon_example(FA, tr("Example 1"));
}

void MainWindow::on_RemoveEpsilon_example1_triggered()
{
    FiniteAutomata FA;
    FA.states << "s" << "q" << "f";
    FA.startState = "s";
    FA.finalStates << "f";
    FA.alphabet << "0" << "1";
    FA.rules << ComputationalRules("s","q","1") << ComputationalRules("q","s","0")
             << ComputationalRules("q","f",EPSILON) << ComputationalRules("f","q",EPSILON)
             << ComputationalRules("f","s","1") << ComputationalRules("s","f","0")   ;
    FA.coordinates["s"] = QPoint(166,292);
    FA.coordinates["q"] = QPoint(266,81);
    FA.coordinates["f"] = QPoint(389,295);
    RemoveEpsilon_example(FA, tr("Example 2"));
}

void MainWindow::on_RemoveEpsilon_example2_triggered()
{
    FiniteAutomata FA;
    FA.states << "s" << "f1" << "f2" << "q1" << "q2" << "q3";
    FA.startState = "s";
    FA.finalStates << "f1" << "f2";
    FA.alphabet << "a" << "b";
    FA.rules << ComputationalRules("s","f1",EPSILON) << ComputationalRules("s","f2",EPSILON)
             << ComputationalRules("f1","q1","a") << ComputationalRules("q1","f1","b")
             << ComputationalRules("f2","q2","a") << ComputationalRules("q2","q3","b")
             << ComputationalRules("q3","f2","a");
    FA.coordinates["s"]  = QPoint(176,196);
    FA.coordinates["f1"] = QPoint(175,113);
    FA.coordinates["f2"] = QPoint(178,293);
    FA.coordinates["q1"] = QPoint(357,114);
    FA.coordinates["q2"] = QPoint(282,189);
    FA.coordinates["q3"] = QPoint(338,295);
    RemoveEpsilon_example(FA, tr("Example 3"));
}

void MainWindow::on_RemoveEpsilon_example3_triggered()
{
    FiniteAutomata FA;
    FA.states << "s" << "q1" << "q2" << "f";
    FA.startState = "s";
    FA.finalStates << "f";
    FA.alphabet << "a" << "b";
    FA.rules
        << ComputationalRules("s","q2","a")
        << ComputationalRules("q1","s","a")
        << ComputationalRules("q1","q2","b")
        << ComputationalRules("q2","f",EPSILON)
        << ComputationalRules("q1","f","b")
        << ComputationalRules("f","q1",EPSILON)
        << ComputationalRules("q1","q2","b");
    FA.coordinates["s"] = QPoint(168,192);
    FA.coordinates["q1"] = QPoint(265,89);
    FA.coordinates["q2"] = QPoint(271,295);
    FA.coordinates["f"] = QPoint(386,202);
    RemoveEpsilon_example(FA, tr("Example 4"));
}

void MainWindow::on_RemoveEpsilon_advanced_example1_triggered()
{
    FiniteAutomata FA;
    FA.states << "s" << "q1" << "q2" << "f";
    FA.startState = "s";
    FA.finalStates << "f";
    FA.alphabet << "a" << "b" <<"c";
    FA.rules
        << ComputationalRules("s","q1",EPSILON)
        << ComputationalRules("s","q2",EPSILON)
        << ComputationalRules("q1","f","b")
        << ComputationalRules("q2","f","c")
        << ComputationalRules("s","s","a")
        << ComputationalRules("q1","q1","b")
        << ComputationalRules("q2","q2","c")
        << ComputationalRules("f","f","a");
    FA.coordinates["s"] = QPoint(169,287);
    FA.coordinates["q1"] = QPoint(280,220);
    FA.coordinates["q2"] = QPoint(279,346);
    FA.coordinates["f"] = QPoint(395,291);
    RemoveEpsilon_example(FA, tr("Advanced example 1"));
}

void MainWindow::on_RemoveEpsilon_advanced_example2_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3" << "4";
    FA.startState = "0";
    FA.finalStates << "2";
    FA.alphabet << "a" << "b";
    FA.rules
        << ComputationalRules("0","1",EPSILON)
        << ComputationalRules("0","3",EPSILON)
        << ComputationalRules("1","2",EPSILON)
        << ComputationalRules("3","2",EPSILON)
        << ComputationalRules("1","1","a")
        << ComputationalRules("2","2","b")
        << ComputationalRules("3","4","a")
        << ComputationalRules("4","3","b");
    FA.coordinates["0"] = QPoint(168,194);
    FA.coordinates["1"] = QPoint(235,114);
    FA.coordinates["2"] = QPoint(319,192);
    FA.coordinates["3"] = QPoint(230,288);
    FA.coordinates["4"] = QPoint(394,294);
    RemoveEpsilon_example(FA, tr("Advanced example 2"));
}

void MainWindow::on_RemoveEpsilon_advanced_example3_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3";
    FA.startState = "0";
    FA.finalStates << "3";
    FA.alphabet << "a" << "b" << "c" << "d";
    FA.rules
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
    FA.coordinates["0"] = QPoint(153,177);
    FA.coordinates["1"] = QPoint(264,109);
    FA.coordinates["2"] = QPoint(273,283);
    FA.coordinates["3"] = QPoint(386,179);
    RemoveEpsilon_example(FA, tr("Advanced example 3"));
}

void MainWindow::on_RemoveEpsilon_advanced_example4_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3" << "4" << "5";
    FA.startState = "0";
    FA.finalStates << "5";
    FA.alphabet << "a" << "b" << "c" << "d";
    FA.rules
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
    FA.coordinates["0"] = QPoint(163,190);
    FA.coordinates["1"] = QPoint(207,108);
    FA.coordinates["2"] = QPoint(209,297);
    FA.coordinates["3"] = QPoint(296,114);
    FA.coordinates["4"] = QPoint(337,296);
    FA.coordinates["5"] = QPoint(382,199);
    RemoveEpsilon_example(FA, tr("Advanced example 4"));
}

void MainWindow::Determinization_example(FiniteAutomata _FA, QString example_name)
{
    on_action_check_mode_triggered();
    ui->action_Determinization->setChecked(true);

    if(m_activeConversion != DFA)
        PrepareDFA();
    DFA_algorithm->setInputFA(_FA);
    mySetWindowTitle(example_name);
}

void MainWindow::on_Determinization_example_1_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1";
    FA.startState = "0";
    FA.finalStates << "1";
    FA.alphabet << "a" << "b";
    FA.rules
        << ComputationalRules("0","0","a")
        << ComputationalRules("0","0","b")
        << ComputationalRules("0","1","b");
    FA.coordinates["0"] = QPoint(178,227);
    FA.coordinates["1"] = QPoint(316,230);
    Determinization_example(FA, tr("Example 1"));
}
void MainWindow::on_Determinization_example_2_triggered()
{
    FiniteAutomata FA;
    FA.states << "A" << "B";
    FA.startState = "A";
    FA.finalStates << "B";
    FA.alphabet << "0" << "1";
    FA.rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","A","1")
        << ComputationalRules("B","B","0")
        << ComputationalRules("A","B","0");
    FA.coordinates["A"] = QPoint(173,198);
    FA.coordinates["B"] =  QPoint(318,200);
    Determinization_example(FA, tr("Example 2"));
}

void MainWindow::on_Determinization_example_3_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3";
    FA.startState = "0";
    FA.finalStates << "3";
    FA.alphabet << "a" << "b";
    FA.rules
        << ComputationalRules("0","0","b")
        << ComputationalRules("0","1","b")
        << ComputationalRules("3","1","b")
        << ComputationalRules("3","1","a")
        << ComputationalRules("2","3","b")
        << ComputationalRules("1","2","b")
        << ComputationalRules("2","1","a");
    FA.coordinates["0"] = QPoint(184,149);
    FA.coordinates["1"] = QPoint(188,249);
    FA.coordinates["2"] = QPoint(186,417);
    FA.coordinates["3"] = QPoint(385,249);
    Determinization_example(FA, tr("Example 3"));
}

void MainWindow::on_Determinization_example_4_triggered()
{
    FiniteAutomata FA;
    FA.states << "s" << "q1" << "q2" << "f";
    FA.startState = "s";
    FA.finalStates << "f";
    FA.alphabet << "a" << "b" <<"c";
    FA.rules
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
    FA.coordinates["s"] = QPoint(169,315);
    FA.coordinates["q1"] = QPoint(279,425);
    FA.coordinates["q2"] = QPoint(277,231);
    FA.coordinates["f"] = QPoint(396,315);
    Determinization_example(FA, tr("Example 4"));
}


void MainWindow::on_Determinization_advanced_example_1_triggered()
{
    FiniteAutomata FA;
    FA.states << "A" << "B" << "C" << "D";
    FA.startState = "A";
    FA.finalStates << "D";
    FA.alphabet << "0" << "1";
    FA.rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","A","1")
        << ComputationalRules("A","B","1")
        << ComputationalRules("B","C","0")
        << ComputationalRules("B","C","1")
        << ComputationalRules("C","D","0")
        << ComputationalRules("C","D","1");
    FA.coordinates["A"] = QPoint(161,279);
    FA.coordinates["B"] = QPoint(246,281);
    FA.coordinates["C"] = QPoint(333,282);
    FA.coordinates["D"] = QPoint(410,282);
    Determinization_example(FA, tr("Advanced example 1"));
}

void MainWindow::on_Determinization_advanced_example_2_triggered()
{
    FiniteAutomata FA;
    FA.states << "A" << "B" << "C";
    FA.startState = "A";
    FA.finalStates << "B";
    FA.alphabet << "0" << "1";
    FA.rules
        << ComputationalRules("A","A","0")
        << ComputationalRules("A","B","1")
        << ComputationalRules("B","A","0")
        << ComputationalRules("C","A","1")
        << ComputationalRules("B","C","0")
        << ComputationalRules("C","B","1");
    FA.coordinates["A"] = QPoint(178,201);
    FA.coordinates["B"] = QPoint(278,402);
    FA.coordinates["C"] = QPoint(378,198);
    Determinization_example(FA, tr("Advanced example 2"));
}

void MainWindow::on_Determinization_advanced_example_3_triggered()
{
    FiniteAutomata FA;
    FA.states << "A" << "B" << "C" << "D" << "E";
    FA.startState = "A";
    FA.finalStates << "D" << "E";
    FA.alphabet << "0" << "1";
    FA.rules
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
    FA.coordinates["A"] = QPoint(167,135);
    FA.coordinates["B"] = QPoint(167,365);
    FA.coordinates["C"] = QPoint(254,244);
    FA.coordinates["D"] = QPoint(286,367);
    FA.coordinates["E"] = QPoint(370,249);
    Determinization_example(FA, tr("Advanced example 3"));
}

void MainWindow::on_Determinization_advanced_example_4_triggered()
{
    FiniteAutomata FA;
    FA.states << "A" << "B" << "C";
    FA.startState = "A";
    FA.finalStates << "C";
    FA.alphabet << "0" << "1";
    FA.rules
        << ComputationalRules("A","A","1")
        << ComputationalRules("A","C","0")
        << ComputationalRules("A","C","1")
        << ComputationalRules("C","A","1")
        << ComputationalRules("C","B","0")
        << ComputationalRules("B","B","1")
        << ComputationalRules("B","A","0")
        << ComputationalRules("B","A","1");
    FA.coordinates["A"] = QPoint(159,266);
    FA.coordinates["B"] = QPoint(309,152);
    FA.coordinates["C"] = QPoint(326,363);
    Determinization_example(FA, tr("Advanced example 4"));
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
    out << m_activeConversion << mode;
    switch(m_activeConversion)
    {
        case RE_to_FA:
        {
            // In play mode does not make sence to save output fa
            if(mode == Algorithm::PLAY_MODE)
            {
                out << reg_exp_algorithm->re->regexp;
            }
            else
            {
                out << *reg_exp_algorithm->re;
            }
        }
        break;
        case REMOVE_EPSILON:
            if(mode == Algorithm::PLAY_MODE)
            {
                 out << remove_epsilon_algorithm->FA;
            }
            else
            {
                out << remove_epsilon_algorithm->FA << remove_epsilon_algorithm->non_epsilon_FA;
            }
        break;
        case DFA:
            if(mode == Algorithm::PLAY_MODE)
            {
                 out << DFA_algorithm->FA;
            }
            else
            {
                out << DFA_algorithm->FA << DFA_algorithm->DFA;
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
    Algorithm::modes tmp_mode = Algorithm::NONE;

    in >> conversion;
    in >> tmp_mode;
    switch (tmp_mode)
    {
        case Algorithm::CHECK_MODE:
            on_action_check_mode_triggered();
        break;
        case Algorithm::PLAY_MODE:
            on_action_play_mode_triggered();
        break;
        case Algorithm::STEP_MODE:
            on_action_step_mode_triggered();
        break;
        case Algorithm::NONE:
            showStatusMessage(tr("ERROR: Input file in wrong format!"));
            return;
        break;
    }
    mode = tmp_mode;

    switch(conversion)
    {
        case RE_to_FA:
        {
            if(mode == Algorithm::PLAY_MODE)
            {
                QString regexp_str;
                in >> regexp_str;
                prepareREtoFA(new RegExp(regexp_str));
            }
            else
            {
                RegExp re;
                in >> re;
                prepareREtoFA(new RegExp(re));
            }
        }
        break;
        case REMOVE_EPSILON:
        {
            FiniteAutomata in_FA;
            in >> in_FA;
            prepareRemoveEpsilon();
            remove_epsilon_algorithm->setInputFA(in_FA);
            if(mode != Algorithm::PLAY_MODE)
            {
                FiniteAutomata out_FA;
                in >> out_FA;
                remove_epsilon_algorithm->setOutputFA(out_FA);
            }
        }
        break;
        case DFA:
        {
            FiniteAutomata in_FA;
            in >> in_FA;
            PrepareDFA();
            DFA_algorithm->setInputFA(in_FA);
            if(mode != Algorithm::PLAY_MODE)
            {
                FiniteAutomata out_FA;
                in >> out_FA;
                DFA_algorithm->setOutputFA(out_FA);
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

void MainWindow::on_actionCFGtoPDA_triggered()
{
    PrepareConversionWidget(CFG_TO_PDA);
}

void MainWindow::SetActionsGroups()
{

    QActionGroup* modesGroup = new QActionGroup(this);
    modesGroup->addAction(ui->action_check_mode);
    modesGroup->addAction(ui->action_play_mode);
    modesGroup->addAction(ui->action_step_mode);

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
