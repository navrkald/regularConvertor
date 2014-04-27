#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "algorithms/htmldelegate.h"
#include "algorithms/algorithmview.h"
#include "algorithms/algorithmwidget.h"
//#include "finite_machine/finiteautomata.h"

#define MY_WINDOW_TITLE "Regular Convertor"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);
    //this->ui->actionSimple_example_1->

    statusBarTimeout = 5000; //5 second
    setWindowTitle(MY_WINDOW_TITLE);
    connect(ui->action_RE_to_FA,SIGNAL(triggered()),this,SLOT(prepareREtoFA()));
    connect(ui->action_RemoveEpsilon,SIGNAL(triggered()),this,SLOT(prepareRemoveEpsilon()));
    connect(ui->action_Determinization,SIGNAL(triggered()),this,SLOT(prepareDFA()));

    QActionGroup* modesGroup = new QActionGroup(this);
    modesGroup->addAction(ui->action_check_mode);
    modesGroup->addAction(ui->action_play_mode);
    modesGroup->addAction(ui->action_step_mode);

    QActionGroup* conversionGroup = new QActionGroup(this);
    conversionGroup->addAction(ui->action_RE_to_FA);
    conversionGroup->addAction(ui->action_RemoveEpsilon);
    conversionGroup->addAction(ui->action_Determinization);

    reg_exp_algorithm = 0;
    remove_epsilon_algorithm = 0;
    activeConversion = none;
    mode = Algorithm::PLAY_MODE;
    ui->action_play_mode->setChecked(true);

    alhgorithm_widget = new AlgorithmWidget(mode);
    alhgorithm_widget->hide();
    connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));

    //    deleteShortCut = new QShortcut(QKeySequence::Delete, this);
    //    connect( deleteShortCut, SIGNAL(activated()), FA1_widget->scene, SLOT(deleteSelected()));


    // Custom status bar
    status_label = new QLabel(this);
    statusBar()->addWidget(status_label);
    status_label->hide();
    status_timer = new QTimer(this);
    status_timer->setInterval(5000);

    status_label->setStyleSheet("QLabel { background-color : white;}");

    connect(status_timer,SIGNAL(timeout()),this,SLOT(hideStatusMessage()));

}

MainWindow::~MainWindow()
{
    delete ui;
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
    if(activeConversion != RE_to_FA)
    {
        activeConversion = RE_to_FA;

        //basic components
        regExpToFA_central_widget = new QWidget(this);
        fa_widget_left = new FA_widget(regExpToFA_central_widget);
        fa_widget_center = new FA_widget(regExpToFA_central_widget);
        fa_widget_right = new FA_widget(regExpToFA_central_widget);
        connect(fa_widget_left,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_widget_center,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_widget_right,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        reg_exp_widget = new RegExpWidget(regExpToFA_central_widget);
        alhgorithm_widget = new AlgorithmWidget(mode,regExpToFA_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
        reg_exp_algorithm = new RegExpToFA(alhgorithm_widget, mode, reg_exp_widget, fa_widget_left, fa_widget_center, fa_widget_right, _re, regExpToFA_central_widget);
        prepareREtoFA_GUI();
    }
    else
    {
        ;
    }
}

void MainWindow::prepareREtoFA_GUI()
{
    setWindowTitle(MY_WINDOW_TITLE + tr(" - Převod regulárního výrazu na konečný automat"));

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
    QLabel* reg_exp_label = new QLabel("<b>Regulární výraz</b>",w);
    reg_exp_label->setAlignment(Qt::AlignCenter);
    reg_exp_vlayout->addWidget(reg_exp_label);
    reg_exp_vlayout->addWidget(reg_exp_widget);

    //algorithm
    QWidget* algorithm_container = prepareAlgorithnContainer(w, "<b>Algoritmus RV na FA</b>", reg_exp_algorithm);

    //left FA
    QWidget* left_fa_container = prepareFAContainer(w, "<b>levý syn</b>", fa_widget_left);

    //center FA
    QWidget* center_fa_container = prepareFAContainer(w, "<b>vybraný uzel</b>", fa_widget_center);

    //right FA
    QWidget* right_fa_container = prepareFAContainer(w, "<b>pravý syn</b>", fa_widget_right);


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



void MainWindow::prepareRemoveEpsilon(FiniteAutomata FA)
{
    if(activeConversion != REMOVE_EPSILON)
    {
        activeConversion = REMOVE_EPSILON;

        //basic components
        removeEpsilon_central_widget = new QWidget(this);
        alhgorithm_widget = new AlgorithmWidget(mode,removeEpsilon_central_widget);
        fa_epsilon_widget = new FA_widget(removeEpsilon_central_widget);
        fa_not_epsilon_widget = new FA_widget(removeEpsilon_central_widget);
        connect(fa_epsilon_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(fa_not_epsilon_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        remove_epsilon_variables_widget = new QLabel(removeEpsilon_central_widget);
        epsilon_closer_list_widget = new QListWidget(removeEpsilon_central_widget);
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
        remove_epsilon_algorithm = new RemoveEpsilon(FA, mode, alhgorithm_widget, fa_epsilon_widget, fa_not_epsilon_widget, remove_epsilon_variables_widget, epsilon_closer_list_widget, removeEpsilon_central_widget);
        prepareRemoveEpsilon_GUI();
    }
    else
    {
        ;
    }
}



void MainWindow::prepareRemoveEpsilon_GUI()
{
    setWindowTitle(MY_WINDOW_TITLE + tr(" - Odstranění epsilon pravidel"));

     delete this->centralWidget();

    //set central widget
    QWidget* w = removeEpsilon_central_widget;
    this->setCentralWidget(w);
    QLayout* layout = new QGridLayout(w);
    layout->setMargin(0);

    //algorithm container
    QWidget* algorithm_container = prepareAlgorithnContainer(w, "<b>Algoritmus Odstranění epsilon pravidel</b>", remove_epsilon_algorithm);

    //input FA container
    QWidget* fa_epsilon_container = prepareFAContainer(w, "<b>vstupní automat</b>", fa_epsilon_widget);

    //output FA container
    QWidget* fa_not_epsilon_container = prepareFAContainer(w, "<b>výstupní automat</b>", fa_not_epsilon_widget);

    //variables container
    QWidget* variables_container = variablesContainer(w, "<b>Proměnné</b>", remove_epsilon_variables_widget);


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


void MainWindow::prepareDFA(FiniteAutomata FA)
{

    if(activeConversion != DFA)
    {
        activeConversion = DFA;

        //basic components
        DFA_central_widget = new QWidget(this);
        alhgorithm_widget = new AlgorithmWidget(mode,DFA_central_widget);   // TODO nevytvaret porad novy ale zmenit parrenta a vymazat
        not_DFA_widget = new FA_widget(DFA_central_widget);
        DFA_widget = new FA_widget(DFA_central_widget);
        connect(not_DFA_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        connect(DFA_widget,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        DFA_variables_widget = new QLabel(DFA_central_widget);
        DFA_variables_widget->setStyleSheet("QLabel { background-color : white; color : black; }");
        connect(this, SIGNAL(modeChanged(Algorithm::modes)), alhgorithm_widget, SLOT(setWidgets(Algorithm::modes)));
        DFA_algorithm = new FaToDFA(FA, mode, alhgorithm_widget, not_DFA_widget, DFA_widget, DFA_variables_widget, DFA_central_widget);
        connect(DFA_algorithm,SIGNAL(sendStatusBarMessage(QString)),this,SLOT(showStatusMessage(QString)));
        prepareDFA_GUI();
    }
    else
    {
        ;
    }
}


void MainWindow::prepareDFA_GUI()
{
    setWindowTitle(MY_WINDOW_TITLE + tr(" - Determinizace KA"));

     delete this->centralWidget();

    //set central widget
    QWidget* w = DFA_central_widget;
    this->setCentralWidget(w);
    QLayout* layout = new QGridLayout(w);
    layout->setMargin(0);

    //algorithm container
    QWidget* algorithm_container = prepareAlgorithnContainer(w, "<b>Determinizace KA</b>", DFA_algorithm);

    //input FA container
    QWidget* in_fa_container = prepareFAContainer(w, "<b>vstupní automat</b>", not_DFA_widget);

    //output FA container
    QWidget* out_fa_container = prepareFAContainer(w, "<b>výstupní automat</b>", DFA_widget);

    //variables container
    QWidget* variables_container = variablesContainer(w, "<b>Proměnné</b>", DFA_variables_widget);
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


/////////////////////////////////////////
/////////////////////////////////////////
// RegExp to FA EXAMPLES
/////////////////////////////////////////
/////////////////////////////////////////

void MainWindow::RE_FA_example(RegExp *_re)
{
    on_action_check_mode_triggered();
    ui->action_RE_to_FA->setChecked(true);
    if(activeConversion != RE_to_FA)
        prepareREtoFA(_re);
    reg_exp_algorithm->setExample(_re);


}

void MainWindow::on_RE_FA_example0_triggered()
{
    RE_FA_example(new RegExp());
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

void MainWindow::RemoveEpsilon_example(FiniteAutomata _FA)
{
    on_action_check_mode_triggered();
    ui->action_RemoveEpsilon->setChecked(true);
    if(activeConversion != REMOVE_EPSILON)
        prepareRemoveEpsilon();
    remove_epsilon_algorithm->setExample(_FA);
}

void MainWindow::on_RemoveEpsilon_example0_triggered()
{
    FiniteAutomata FA;
    FA.states << "0" << "1" << "2" << "3";
    FA.startState = "0";
    FA.finalStates << "2" << "3";
    FA.alphabet << "a" << "b";
    FA.rules << ComputationalRules("0","1",EPSILON) << ComputationalRules("1","2","a") << ComputationalRules("1","3","b");
    RemoveEpsilon_example(FA);
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
    RemoveEpsilon_example(FA);
}

void MainWindow::Determinization_example(FiniteAutomata _FA)
{
    on_action_check_mode_triggered();
    ui->action_Determinization->setChecked(true);

    if(activeConversion != DFA)
    {
        prepareDFA();
    }
    DFA_algorithm->setExample(_FA);
}

void MainWindow::on_Determinization_example_1_triggered()
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

    Determinization_example(FA);
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

    Determinization_example(FA);
}
