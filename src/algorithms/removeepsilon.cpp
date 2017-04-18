#include "removeepsilon.h"

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"

#define HEADER 0
#define FOREACH_P_EPSILONCLOSER 1
#define EPSILONCLOSER 2
#define FOREACH_P 3
#define FOREACH_P_IN_CLOSER 4
#define FOREACH_RULE 5
#define NEW_RULE 6
#define FOREACH_P_FINAL 7
#define IF_FINAL 8
#define NEW_FINAL 9
#define LAST_INSTRUCTION 10

RemoveEpsilon::RemoveEpsilon(FiniteAutomata _FA)
{
    this->FA = _FA;
}

RemoveEpsilon::RemoveEpsilon(AlgorithmModes _mode, CAlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, CVariablesWidget* _var_widget, QListWidget* _epsilon_closer_list_widget, QObject* parrent)
 :  CAlgorithm(parrent), mode(_mode)
{
    Init(_algorithm_widget, _epsilon_fa_widget, _not_epsilon_fa_widget, _var_widget, _epsilon_closer_list_widget);
}

void RemoveEpsilon::Init(CAlgorithmWidget *_algorithm_widget, FA_widget *_epsilon_fa_widget, FA_widget *_not_epsilon_fa_widget, CVariablesWidget *_var_widget, QListWidget *_epsilon_closer_list_widget)
{
    m_algorithmWidget = _algorithm_widget;
    epsilon_fa_widget = _epsilon_fa_widget;
    not_epsilon_fa_widget = _not_epsilon_fa_widget;
    var_widget = _var_widget;
    epsilon_closer_list_widget = _epsilon_closer_list_widget;
    SetupAlgorithmWidget(m_algorithmWidget);

    m_actInstruction = HEADER;
    m_prewInstruction = HEADER;
    m_instructionCount = NEW_FINAL+1;



    InitInstructions();
    InitBreakpoints(m_instructionCount);

    QIcon epsilon_closer_icon = QIcon(tr(":/algorithms/algorithms/pictures/epsilon_closer_en.png"));
    QIcon remove_epsilon_icon = QIcon(tr(":/algorithms/algorithms/pictures/remove_epsilon_en.png"));

    this->setColumnCount(1);
    this->setRowCount(m_instructions.count());

    var_widget->SetText("");

    for(int i = 0; i < m_instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,m_instructions[i],Qt::EditRole);
        setData(index,true,CAlgorithm::hasBreakpointRole);
        setData(index,false,CAlgorithm::breakpointRole);
        switch(i)
        {
            case HEADER:
                setData(index,false,CAlgorithm::hasBreakpointRole);
                break;
            case EPSILONCLOSER:
                setData(index,epsilon_closer_icon,Qt::DecorationRole);
                break;
            case NEW_RULE:
                setData(index,remove_epsilon_icon,Qt::DecorationRole);
                break;
        }
    }


    //
    // Connect algorithm buttons.
    //
    connect(this->m_algorithmWidget,SIGNAL(playPressed(int)),this,SLOT(RunAlgorithm(int)));
    connect(this->m_algorithmWidget,SIGNAL(stopPressed()),this,SLOT(Stop()));
    connect(this->m_algorithmWidget,SIGNAL(prewPressed()),this,SLOT(prevStep()));
    connect(this->m_algorithmWidget,SIGNAL(nextPressed()),this,SLOT(nextStep()));
    connect(this->m_algorithmWidget, SIGNAL(checkSolutionPressed()), this, SLOT(checkSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(showCorrectSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showUserSolutionPressed()), this, SLOT(showUserSolution()));
    connect(this->m_algorithmWidget, SIGNAL(beginPressed()), this, SLOT(toBegin()));
    connect(this->m_algorithmWidget, SIGNAL(endPressed()), this, SLOT(toEnd()));

    //
    // Connect timers.
    //
    connect(m_playTimer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(m_CheckStepTimer, SIGNAL(timeout()), this, SLOT(checkSolution()));

    // Connect Finite Automata widgets
    connect(epsilon_fa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setFA(FiniteAutomata*)));
    connect(not_epsilon_fa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(set_not_epsilonFA(FiniteAutomata*)));

    epsilon_fa_widget->setFA(new FiniteAutomata());
    not_epsilon_fa_widget->setFA(new FiniteAutomata());
    //here is implemented
    m_algorithmWidget->enableShowButton();
}

void RemoveEpsilon::InitInstructions()
{
    m_instructions.resize(m_instructionCount);
    m_instructions[HEADER] =                     tr("<b>Input:</b> FA <i>M</i>=(Q,Σ,R,s,F)<br><b>Output:</b> FA without ε-rules M'=(Q,Σ,R',s,F')");
    m_instructions[FOREACH_P_EPSILONCLOSER] =    tr("<b>for each</b> p ∈ Q <b>do</b>");
    m_instructions[EPSILONCLOSER] =              INDENT + tr("ε-closer(p) = {q:q∈Q, p ⊢* q}");

    m_instructions[FOREACH_P] =                  tr("<b>for each</b> p ∈ Q <b>do</b>");
    m_instructions[FOREACH_P_IN_CLOSER] =        INDENT     + tr("<b>for each</b> p'∈ ε-closer(p) <b>do</b>");
    m_instructions[FOREACH_RULE] =               INDENT INDENT     + tr("<b>for each</b> r={p'a→q}∈ R <b>where</b> a ∈ Σ <b>do</b>");
    m_instructions[NEW_RULE] =                   INDENT INDENT INDENT     + tr("R'∈ R' ∪ r'={pa→q}");

    m_instructions[FOREACH_P_FINAL] =            tr("<b>for each</b> p ∈ Q <b>do</b>");
    m_instructions[IF_FINAL] =                   INDENT      + tr("<b>if</b> ε-closer(p) ∩ F ≠ ∅ <b>then</b>");
    m_instructions[NEW_FINAL] =                  INDENT INDENT       +tr ("F'∈ F' ∪ {p}");
}

void RemoveEpsilon::setFA(FiniteAutomata *FA)
{
    this->FA = *FA;
    not_epsilon_fa_widget->clearStatus();
    SetMode(mode);
}

void RemoveEpsilon::set_not_epsilonFA(FiniteAutomata *FA)
{
    this->non_epsilon_FA = *FA;
}

void RemoveEpsilon::SetMode(AlgorithmModes _mode)
{
    mode = _mode;

    m_num = 0;
    m_actInstruction=HEADER;
    m_prewInstruction=HEADER;
    ClearActInstruction();
    clearVariables();
    epsilon_closer_list_widget->clear();
    // because show correct solution break connectiom betwen user FA and user FA widget
    connect(not_epsilon_fa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(set_not_epsilonFA(FiniteAutomata*)));

    switch (mode)
    {
        case PLAY_MODE:
            m_algorithmWidget->enableNext();
            m_algorithmWidget->disablePrev();
            not_epsilon_fa_widget->setFA(new FiniteAutomata());
            //unselect instruction from algorithm window
            ClearActInstruction();
            history.clear();
            m_actPos = 0;
            m_actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            saveStep();
        break;
        case CHECK_MODE: case STEP_MODE:
            correct_FA = computeSolution();
        break;
        case NONE:
        break;
    }

    if(mode == STEP_MODE)
        m_CheckStepTimer->start(CHECK_STEP_TIMEOUT);
    else
        m_CheckStepTimer->stop();

    not_epsilon_fa_widget->clearStatus();
}

void RemoveEpsilon::nextStep()
{
    m_algorithmWidget->enablePrev();
    switch(m_prewInstruction)
    {
//
// Epsilon Closer
//
        case HEADER:
            // non_epsilon_FA initialization
            non_epsilon_FA = FiniteAutomata();
            non_epsilon_FA.m_states = FA.m_states;
            non_epsilon_FA.m_startState = FA.m_startState;
            non_epsilon_FA.m_alphabet = FA.m_alphabet;
            non_epsilon_FA.m_coordinates = FA.m_coordinates;
            not_epsilon_fa_widget->setFA(new FiniteAutomata(non_epsilon_FA));
            p_list = FA.get_sorted_states();
            if(!p_list.empty())
                m_actInstruction = FOREACH_P_EPSILONCLOSER;
            else
                m_actInstruction = lastInstruction;
        break;
        case FOREACH_P_EPSILONCLOSER:
            m_actInstruction = EPSILONCLOSER;
        break;
        case EPSILONCLOSER:
            if(p_list.count()>0)
            {
                m_actInstruction = FOREACH_P_EPSILONCLOSER;
            }
            else
            {
                p_list = FA.get_sorted_states();
                if(!p_list.empty())
                    m_actInstruction = FOREACH_P;
                else
                    m_actInstruction = lastInstruction;
            }
        break;
//
// Remove Epsilon
//
        case FOREACH_P:
            // alwais at leas one
            p_prime_list = FA.epsilonCloser(p).toList();
            epsilon_closer = p_prime_list;
            m_actInstruction = FOREACH_P_IN_CLOSER;
        break;
        case FOREACH_P_IN_CLOSER:
             non_epsilon_rule_list = FA.nonEpsilonRulesOf(p_prime).toList();
             if(! non_epsilon_rule_list.empty())
             {
                m_actInstruction = FOREACH_RULE;
             }
             else if (!p_prime_list.empty())
             {
                 m_actInstruction = FOREACH_P_IN_CLOSER;
             }
             else if (! p_list.empty())
             {
                 m_actInstruction = FOREACH_P;
             }
             else
             {
                 p_list = FA.get_sorted_states();
                 if(!p_list.empty())
                     m_actInstruction = FOREACH_P_FINAL;
                 else
                     m_actInstruction = lastInstruction;
             }
        break;
        case FOREACH_RULE:
            m_actInstruction = NEW_RULE;
        break;
        case NEW_RULE:
        {
            if(!non_epsilon_rule_list.empty())
            {
               m_actInstruction = FOREACH_RULE;
            }
            else if (!p_prime_list.empty())
            {
                m_actInstruction = FOREACH_P_IN_CLOSER;
            }
            else if (!p_list.empty())
            {
                m_actInstruction = FOREACH_P;
            }
            else
            {
                p_list = FA.get_sorted_states();
                if(!p_list.empty())
                    m_actInstruction = FOREACH_P_FINAL;
                else
                    m_actInstruction = lastInstruction;
            }
        }
        break;
        case FOREACH_P_FINAL:
            epsilon_closer = FA.epsilonCloser(p).toList();
            m_actInstruction = IF_FINAL;
        break;
        case IF_FINAL:
            if(!epsilon_closer.toSet().intersect(FA.m_finalStates).empty())
            {
                m_actInstruction = NEW_FINAL;
            }
            else if (!p_list.empty())
            {
                m_actInstruction = FOREACH_P_FINAL;
            }
            else
            {
                m_actInstruction = lastInstruction;
            }
        break;
        case NEW_FINAL:
            if (!p_list.empty())
            {
                m_actInstruction = FOREACH_P_FINAL;
            }
            else
            {
                m_actInstruction = lastInstruction;
            }
        break;
    }

    switch(m_actInstruction)
    {
        case FOREACH_P_EPSILONCLOSER:

            p = p_list.first();
            p_list.pop_front();
        break;
        case EPSILONCLOSER:
        {
            epsilon_closer = FA.epsilonCloser(p).toList();
            qSort(epsilon_closer);
            QPair <QString ,QList <QString> > e_closer;
            e_closer.first = p;
            e_closer.second = epsilon_closer;
            epsilon_closers.append(e_closer);
            QListWidgetItem * item = new QListWidgetItem(epsilon_closer_list_widget);
            EpsilonCloserWidget* epsilon_widget = new EpsilonCloserWidget(p,epsilon_closer, epsilon_closer, epsilon_closer_list_widget);
            QStringList statesList = FA.m_states.toList();
            epsilon_widget->setCompleter(new MultiSelectCompleter(statesList));
            epsilon_widget->setCorrectness(true);
            epsilon_closer_list_widget->setItemWidget(item,epsilon_widget);
            item->setSizeHint(epsilon_widget->sizeHint());
        }
        break;
        case FOREACH_P:
            p = p_list.first();
            p_list.pop_front();
        break;
        case FOREACH_P_IN_CLOSER:
            p_prime = p_prime_list.first();
            p_prime_list.pop_front();
        break;
        case FOREACH_RULE:
            if( ! non_epsilon_rule_list.isEmpty())
            {
                non_epsilon_rule = non_epsilon_rule_list.at(0);
                non_epsilon_rule_list.pop_front();
            }

        break;
        case NEW_RULE:
            non_epsilon_prime_rule = non_epsilon_rule;
            non_epsilon_prime_rule.from = p;
            non_epsilon_FA.addRule(non_epsilon_prime_rule);
            not_epsilon_fa_widget->emitAddEdge(non_epsilon_prime_rule);
        break;
        case FOREACH_P_FINAL:
            p = p_list.first();
            p_list.pop_front();
        break;
        case IF_FINAL:

        break;
        case NEW_FINAL:
            non_epsilon_FA.m_finalStates.insert(p);
            not_epsilon_fa_widget->emitAddEndingNode(p);
        break;
        case LAST_INSTRUCTION:
        break;
    }

    RemoveFuture();
    if(m_actInstruction != lastInstruction)
    {
        m_prewInstruction = m_actInstruction;
        saveStep();
        if(m_breakpoints[m_actInstruction])
            m_playTimer->stop();
    }
    else
    {
        m_algorithmWidget->disableNext();
        m_playTimer->stop();
        not_epsilon_fa_widget->setCorrectStatus();
    }
    showVariables();
    SetActInstruction();
}

//void RemoveEpsilon::runAlgorithm(int mil_sec)
//{

//}

void RemoveEpsilon::saveStep()
{
    steps s;
    s.num = ++m_num;
    s.actInstruction = m_actInstruction;
    s.prewInstruction = m_prewInstruction;
    s.non_epsilon_FA = non_epsilon_FA;
    s.p = p;
    s.p_prime = p_prime;
    s.non_epsilon_rule = non_epsilon_rule;
    s.non_epsilon_prime_rule = non_epsilon_prime_rule;
    s.epsilon_closer = epsilon_closer;
    s.epsilon_closers = epsilon_closers;
    s.p_list = p_list;
    s.p_prime_list = p_prime_list;
    s.non_epsilon_rule_list = non_epsilon_rule_list;
    history.append(s);
    m_actPos = history.count() - 1;
}

void RemoveEpsilon::prevStep()
{
    if (m_actPos > 0)
    {
        m_algorithmWidget->enableNext();
        m_actPos--;
        steps s = history.at(m_actPos);
        m_num = s.num;
        m_actInstruction = s.actInstruction;
        m_prewInstruction = s.prewInstruction;
        non_epsilon_FA = s.non_epsilon_FA;
        not_epsilon_fa_widget->setFA(new FiniteAutomata(non_epsilon_FA));
        p = s.p;
        p_prime = s.p_prime;
        non_epsilon_rule = s.non_epsilon_rule;
        non_epsilon_prime_rule = s.non_epsilon_prime_rule;
        epsilon_closer = s.epsilon_closer;
        epsilon_closers = s.epsilon_closers;
        p_list = s.p_list;
        p_prime_list = s.p_prime_list;
        non_epsilon_rule_list = s.non_epsilon_rule_list;
        showVariables();
        SetActInstruction();
        setEpsilonCloserWidget();
        not_epsilon_fa_widget->clearStatus();
    }
    else
    {
        m_algorithmWidget->disablePrev();
    }
}

void RemoveEpsilon::setEpsilonCloserWidget()
{
    epsilon_closer_list_widget->clear();
    QPair <QString, QList<QString> > par;
    foreach(par, epsilon_closers)
    {
        QListWidgetItem * item = new QListWidgetItem(epsilon_closer_list_widget);
        EpsilonCloserWidget* epsilon_widget = new EpsilonCloserWidget(par.first, par.second, par.second, epsilon_closer_list_widget);
        QStringList statesList = FA.m_states.toList();
        epsilon_widget->setCompleter(new MultiSelectCompleter(statesList));
        epsilon_widget->setCorrectness(true);
        epsilon_closer_list_widget->setItemWidget(item,epsilon_widget);
        item->setSizeHint(epsilon_widget->sizeHint());
    }
}

void RemoveEpsilon::prepareGUItoCheck()
{
    QStringList states = FA.m_states.toList();
    states.sort();
    epsilon_closer_list_widget->clear();
    foreach(QString state, FA.m_states)
    {
        QStringList closer = FA.epsilonCloser(state).toList();
        closer.sort();
        QListWidgetItem * item = new QListWidgetItem(epsilon_closer_list_widget);
        QStringList emptyList;
        EpsilonCloserWidget* epsilon_widget = new EpsilonCloserWidget(state, emptyList, closer, epsilon_closer_list_widget);
        epsilon_widget->setCompleter(new MultiSelectCompleter(FA.m_states.toList()));
        epsilon_closer_list_widget->setItemWidget(item,epsilon_widget);
        item->setSizeHint(epsilon_widget->sizeHint());
    }
}

void RemoveEpsilon::RemoveFuture()
{
    int count  = history.count();
    for(int i = m_actPos+1; i < count; i++)
    {
        history.removeLast();
    }
}



void RemoveEpsilon::clearVariables()
{
    var_widget->Clear();
}

void RemoveEpsilon::SetInputFA(FiniteAutomata _FA)
{
    epsilon_fa_widget->setFA(new FiniteAutomata(_FA));
}

void RemoveEpsilon::SetOutputFA(FiniteAutomata out_FA)
{
    not_epsilon_fa_widget->setFA(new FiniteAutomata(out_FA));
}

const FiniteAutomata &RemoveEpsilon::GetInputFA()
{
    return *(epsilon_fa_widget->m_finiteAutomata);
}

const FiniteAutomata &RemoveEpsilon::GetOutputFA()
{
    return *(not_epsilon_fa_widget->m_finiteAutomata);
}

void RemoveEpsilon::showVariables()
{

    int instruction;
    if(m_actInstruction == lastInstruction)
        instruction = m_prewInstruction;
    else
        instruction = m_actInstruction;

    var_widget->Clear();
    QString text;
    switch(instruction)
    {
        case HEADER:
            text = "";
            break;

        case FOREACH_P_EPSILONCLOSER:
            text = p_toString();                        // p =
            break;

        case EPSILONCLOSER:
            text = p_toString();                        // p =
            break;

        case FOREACH_P:
            text = p_toString();                        // p =
            break;

        case FOREACH_P_IN_CLOSER:
            text =  p_toString() + "<br>";                       // p =
            text += epsilon_closer_toString() + "<br>";          // e-closer(p)
            text += p_prime_toString();
            break;
        case FOREACH_RULE:
            text = p_toString() + "<br>";                        // p =
            text += epsilon_closer_toString() + "<br>";          // e-closer(p)
            text += p_prime_toString() + "<br>";                 // p' =
            text += symbol_toString() + "<br>";                  // a =
            text += q_toString() + "<br>";                       // q =
            text += non_epsilon_rule_toString();        // r  =
            break;

        case NEW_RULE:
            text = p_toString() + "<br>";                        // p =
            text += epsilon_closer_toString() + "<br>";          // e-closer(p)
            text += p_prime_toString() + "<br>";                 // p' =
            text += symbol_toString() + "<br>";                  // a =
            text += q_toString() + "<br>";                       // q =
            text += non_epsilon_rule_toString() + "<br>";        // r  =
            text += non_epsilon_prime_rule_toString();  // r' =
            break;

        case FOREACH_P_FINAL:
            text = p_toString();                        // p =
            break;

        case IF_FINAL:
            text =  p_toString() + "<br>";                       // p =
            text += epsilon_closer_toString() + "<br>";          // e-closer(p)
            text += F_toString();                       // F =
            break;

        case NEW_FINAL:
            text =  p_toString() + "<br>";                       // p =
            text += epsilon_closer_toString() + "<br>";          // e-closer(p) =
            text += F_toString() + "<br>";                       // F =
            text += F_prime_toString();                 // F'=
        break;
    }
    var_widget->SetText(text);
}



//void RemoveEpsilon::stop()
//{

//}



void RemoveEpsilon::showCorrectSolution()
{
    backup_FA = non_epsilon_FA;
    not_epsilon_fa_widget->setFA(new FiniteAutomata(correct_FA));
}

void RemoveEpsilon::showUserSolution()
{
    not_epsilon_fa_widget->setFA(new FiniteAutomata(backup_FA));
}

void RemoveEpsilon::toBegin()
{
    m_algorithmWidget->enableNext();
    m_actPos = 0;
    steps s = history.at(m_actPos);
    m_num = s.num;
    m_actInstruction = s.actInstruction;
    m_prewInstruction = s.prewInstruction;
    non_epsilon_FA = s.non_epsilon_FA;
    not_epsilon_fa_widget->setFA(new FiniteAutomata(non_epsilon_FA));
    p = s.p;
    p_prime = s.p_prime;
    non_epsilon_rule = s.non_epsilon_rule;
    non_epsilon_prime_rule = s.non_epsilon_prime_rule;
    epsilon_closer = s.epsilon_closer;
    epsilon_closers = s.epsilon_closers;
    p_list = s.p_list;
    p_prime_list = s.p_prime_list;
    non_epsilon_rule_list = s.non_epsilon_rule_list;
    showVariables();
    SetActInstruction();
    setEpsilonCloserWidget();
    not_epsilon_fa_widget->clearStatus();
    m_algorithmWidget->disablePrev();
}

void RemoveEpsilon::toEnd()
{
    while(m_actInstruction != lastInstruction)
        nextStep();
}

void RemoveEpsilon::checkSolution()
{
    if(FiniteAutomata::areEquivalent(non_epsilon_FA, correct_FA) && !non_epsilon_FA.hasEpsilon())
    {
        not_epsilon_fa_widget->setCorrectStatus();
    }
    else
    {
        not_epsilon_fa_widget->setWrongStatus();
    }
}

FiniteAutomata RemoveEpsilon::computeSolution()
{
    FiniteAutomata FA_not_epsilon;
    FA_not_epsilon.m_states = FA.m_states;
    FA_not_epsilon.m_alphabet = FA.m_alphabet;
    FA_not_epsilon.m_startState = FA.m_startState;

    //epsilon closers
    QStringList states = FA.m_states.toList();
    states.sort();
    foreach(QString p,states)
    {
        QStringList e_closer = FA.epsilonCloser(p).toList();
        e_closer.sort();
        correct_epsilon_closers.append(QPair<QString, QList<QString> >(p,e_closer));
    }

    //Rules
    foreach(QString state,FA.m_states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1,epsilon_closer)
        {
            QSet <ComputationalRules> rules = FA.nonEpsilonRulesOf(state1);
            foreach(ComputationalRules rule,rules)
            {
                ComputationalRules newRule = rule;
                newRule.from = state;
                FA_not_epsilon.addRule(newRule);
            }
        }
    }

    //Final states
    foreach(QString state,FA.m_states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1, epsilon_closer)
        {
            if(FA.m_finalStates.contains(state1))
            {
                FA_not_epsilon.addFinalState(state);
            }
        }
    }
    return FA_not_epsilon;
}
