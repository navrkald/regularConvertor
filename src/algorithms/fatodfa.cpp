#include "fatodfa.h"

#define HEADER              0
#define VAR_INIT            1
#define DO                  2
#define DO_INIT             3
#define FOREACH_A           4
#define INIT_DOUBLE_PRIME_Q 5
#define FOREACH_RULE_IN_Q   6
#define DOUBLE_PRIME_Q      7
#define IF_Q_NEW            8
#define IF_DOUBLE_PRIME_Q   9
#define IF_FINAL            10
#define WHILE_NEW           11

FaToDFA::FaToDFA(FiniteAutomata _FA) : FA(_FA), m_mode(AlgorithmModes::NONE)
{
    ;
}

FaToDFA::~FaToDFA()
{

}

FaToDFA::FaToDFA(AlgorithmModes mode, CAlgorithmWidget* _algorithm_widget, FA_widget* _not_dfa_widget, FA_widget* _dfa_widget, CVariablesWidget* _var_widget, QObject* parrent)
 : CAlgorithm(parrent), m_mode(mode)
{
    Init(_algorithm_widget, _not_dfa_widget, _dfa_widget, _var_widget);
}

void FaToDFA::Init(CAlgorithmWidget *_algorithm_widget, FA_widget *_not_dfa_widget, FA_widget *_dfa_widget, CVariablesWidget *_var_widget)
{
    m_algorithmWidget = _algorithm_widget;
    m_not_dfa_widget = _not_dfa_widget;
    m_dfa_widget = _dfa_widget;
    m_var_widget = _var_widget;
    SetupAlgorithmWidget(m_algorithmWidget);


    m_actInstruction = HEADER;
    m_prevInstruction = HEADER;
    m_instructionCount = WHILE_NEW+1;
    InitInstructions();
    InitBreakpoints(m_instructionCount);

    this->setColumnCount(1);
    this->setRowCount(m_instructions.count());

    m_var_widget->SetText("");

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
        }
    }


    //
    // Connect algorithm buttons.
    //
    connect(this->m_algorithmWidget,SIGNAL(playPressed(int)),this,SLOT(RunAlgorithm(int)));
    connect(this->m_algorithmWidget,SIGNAL(stopPressed()),this,SLOT(Stop()));
    connect(this->m_algorithmWidget,SIGNAL(prewPressed()),this,SLOT(PrevStep()));
    connect(this->m_algorithmWidget,SIGNAL(nextPressed()),this,SLOT(NextStep()));
    connect(this->m_algorithmWidget, SIGNAL(checkSolutionPressed()), this, SLOT(CheckSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(ShowCorrectSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showUserSolutionPressed()), this, SLOT(ShowUserSolution()));
    connect(this->m_algorithmWidget, SIGNAL(beginPressed()), this, SLOT(ToBegin()));
    connect(this->m_algorithmWidget, SIGNAL(endPressed()), this, SLOT(ToEnd()));

    //
    // Connect timers.
    //
    connect(m_playTimer, SIGNAL(timeout()), this, SLOT(NextStep()));
    connect(m_CheckStepTimer, SIGNAL(timeout()), this, SLOT(CheckSolution()));

    // Connect Finite Automata widgets
    connect(m_not_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(SetFA(FiniteAutomata*)));
    connect(m_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(SetDFA(FiniteAutomata*)));

    m_not_dfa_widget->setFA(new FiniteAutomata());

    m_algorithmWidget->enableShowButton();
}



void FaToDFA::InitInstructions()
{
    m_instructions.resize(m_instructionCount);
    m_instructions[HEADER] =                  tr("<b>Input:</b> FA without ε-rules <i>M</i>=(Q, Σ, R, s, F)<br><b>Output:</b> DKA M'=(Q<sub>d</sub>, Σ, R<sub>d</sub>, s<sub>d</sub>, F<sub>d</sub>)");
    m_instructions[VAR_INIT] =                tr("s<sub>d</sub> = {s}; Q<sub>new</sub> = {s<sub>d</sub>}; R<sub>d</sub> = ∅; Q<sub>d</sub> = Q<sub>new</sub>; F<sub>d</sub> = ∅");
    m_instructions[DO] =                      tr("<b>do</b>");
    m_instructions[DO_INIT] =                 INDENT     + tr("Q' ∈ Q<sub>new</sub>; Q<sub>new</sub> = Q<sub>new</sub> - {Q'};");
    m_instructions[FOREACH_A] =               INDENT     + tr("<b>for each</b> a ∈ Σ <b>do</b>");
    m_instructions[INIT_DOUBLE_PRIME_Q]=      INDENT  INDENT      + tr("Q'' = ∅");
    m_instructions[FOREACH_RULE_IN_Q] =       INDENT  INDENT     + tr("<b>for each</b> r = {p a → q} <b>where</b> p ∈ Q'");
    m_instructions[DOUBLE_PRIME_Q] =          INDENT  INDENT  INDENT     + tr("Q'' ∈ Q'' ∪ {q}");
    m_instructions[IF_Q_NEW] =                INDENT  INDENT     + tr("<b>if</b> Q'' ∉ Q<sub>d</sub> ∪ {∅} <b>then</b> Q<sub>new</sub> = Q<sub>new</sub> ∪ {Q''}; Q<sub>d</sub> = Q<sub>d</sub> ∪ {Q''};");
    m_instructions[IF_DOUBLE_PRIME_Q] =       INDENT  INDENT     + tr("<b>if</b> Q'' ≠ ∅ <b>then</b> R<sub>d</sub> = R<sub>d</sub> ∪ r'={Q' a → Q''};");
    m_instructions[IF_FINAL] =                INDENT      + tr("<b>if</b> Q' ∩ F ≠ ∅ <b>then</b> F<sub>d</sub> = F<sub>d</sub> ∪ {Q'}");
    m_instructions[WHILE_NEW] =               tr("<b>until</b> Q<sub>new</sub> = ∅");
}

void FaToDFA::SetFA(FiniteAutomata *_FA)
{
    this->FA = *_FA;
    if(this->FA.hasEpsilon())
        emit SendStatusBarMessage(tr("WARNING: Input finite automata has epsilon rules."));
    m_not_dfa_widget->clearStatus();
    SetMode(m_mode);
}

void FaToDFA::SetMode(AlgorithmModes _mode)
{
    m_mode = _mode;
    m_playTimer->stop();
    m_num = 0;
    m_actInstruction=HEADER;
    m_prevInstruction=HEADER;

    ClearActInstruction();
    clearVariables();
    // because show correct solution break connectiom betwen user FA and user FA widget
    connect(m_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(SetDFA(FiniteAutomata*)));

    switch (m_mode)
    {
        case algorithmSteping:
            m_algorithmWidget->enableNext();
            m_algorithmWidget->disablePrev();
            m_dfa_widget->setFA(new FiniteAutomata());
            //unselect instruction from algorithm window
            ClearActInstruction();
            m_history.clear();
            m_actPos = 0;
            m_actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            saveStep();
        break;
        case individualWork:
        case instantChecking:
            m_correct_FA = computeSolution();
        break;
        case NONE:
        break;
    }

    if(m_mode == instantChecking)
        m_CheckStepTimer->start(CHECK_STEP_TIMEOUT);
    else
        m_CheckStepTimer->stop();

    m_dfa_widget->clearStatus();
}

void FaToDFA::SetDFA(FiniteAutomata *_FA)
{
    this->DFA = *_FA;
}

void FaToDFA::NextStep()
{
    if(FA.m_startState == "")
    {
        emit SendStatusBarMessage("WARNING: Input FA has not start state.");
        return;
    }
    if(FA.hasEpsilon())
    {
        emit SendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return;
    }

    m_algorithmWidget->enablePrev();
    switch(m_prevInstruction)
    {
        case HEADER:
            m_actInstruction = VAR_INIT;
        break;
        case VAR_INIT:
            m_actInstruction = DO;
        break;
        case DO:
            m_actInstruction = DO_INIT;
        break;
        case DO_INIT:
            m_alphabet = FA.m_alphabet.toList();
            m_alphabet.sort();
            if( ! m_alphabet.empty())
                m_actInstruction = FOREACH_A;
            else
                m_actInstruction = IF_FINAL;
            break;
        case FOREACH_A:
            m_actInstruction = INIT_DOUBLE_PRIME_Q;
        break;
        case INIT_DOUBLE_PRIME_Q:
            m_rules = QSet<ComputationalRules>();
            foreach(QString from, m_act_state)
            {
                m_rules += FA.findRule_FromSymbol(from,m_a).toSet();
            }
            if( ! m_rules.empty())
                m_actInstruction = FOREACH_RULE_IN_Q;
            else
                m_actInstruction = IF_Q_NEW;
        break;
        case FOREACH_RULE_IN_Q:
            m_actInstruction = DOUBLE_PRIME_Q;
        break;
        case DOUBLE_PRIME_Q:
            if( ! m_rules.empty())
                m_actInstruction = FOREACH_RULE_IN_Q;
            else
                m_actInstruction = IF_Q_NEW;
        break;
        case IF_Q_NEW:
            m_actInstruction = IF_DOUBLE_PRIME_Q;
        break;
        case IF_DOUBLE_PRIME_Q:
            if ( ! m_alphabet.empty())
                m_actInstruction = FOREACH_A;
            else
                m_actInstruction = IF_FINAL;
        break;
        case IF_FINAL:
            m_actInstruction = WHILE_NEW;
        break;
        case WHILE_NEW:
            if ( ! m_Q_new.empty())
                m_actInstruction = DO;
            else
                m_actInstruction = lastInstruction;
        break;
    }

    switch(m_actInstruction)
    {
        case VAR_INIT:
            {
                DFA.m_startState = "{" + FA.m_startState + "}";
                DFA.m_alphabet = FA.m_alphabet;
                DFA.m_states.insert(DFA.m_startState);
                m_dfa_widget->setFA(new FiniteAutomata(DFA));         //display start state
                QSet<QString> tmp_set; tmp_set.insert(FA.m_startState);
                m_Q_new.insert(tmp_set);
            }
            break;
        case DO:
            ;
            break;
        case DO_INIT:
            {
                m_act_state = *(m_Q_new.begin());
                m_Q_new.remove(m_act_state);
                m_act_state_name = qSetToQString(m_act_state);
            }
            break;
        case FOREACH_A:
            m_a = m_alphabet.first();
            m_alphabet.pop_front();
            break;
        case INIT_DOUBLE_PRIME_Q:
            {
                m_discovered_state = QSet<QString>();

            }
            break;
        case FOREACH_RULE_IN_Q:
            m_r = *(m_rules.begin());
            m_rules.remove(m_r);
            m_p = m_r.from;
            m_q = m_r.to;
        break;
        case DOUBLE_PRIME_Q:
            m_discovered_state.insert(m_r.to);
        break;
        case IF_Q_NEW:
        {
            QString discovered_state_name = qSetToQString(m_discovered_state);
            if( ! DFA.m_states.contains(QSet<QString>() << discovered_state_name) && ! m_discovered_state.empty())
            {
                m_Q_new.insert(m_discovered_state);
                DFA.m_states.insert(discovered_state_name);
                m_dfa_widget->addNodes(QSet<QString>() << discovered_state_name);
            }
        }
        break;
        case IF_DOUBLE_PRIME_Q:
            if( ! m_discovered_state.empty())
            {
                m_r_prime = ComputationalRules(qSetToQString(m_act_state),qSetToQString(m_discovered_state),m_a);
                DFA.m_rules.insert(m_r_prime);
                emit m_dfa_widget->addEdges(QSet<ComputationalRules>() << m_r_prime);
            }
        break;
        case IF_FINAL:
            if ( ! (m_act_state & FA.m_finalStates).empty())
            {
                DFA.m_finalStates.insert(qSetToQString(m_act_state));
                m_dfa_widget->addEndingNodes(QSet<QString>() << qSetToQString(m_act_state));
            }
        break;
        case WHILE_NEW:
            ;
        break;
    }

    RemoveFuture();
    if(m_actInstruction != lastInstruction)
    {
        m_prevInstruction = m_actInstruction;
        saveStep();
        if(m_breakpoints[m_actInstruction])
            m_playTimer->stop();
    }
    else
    {
        m_algorithmWidget->disableNext();
        m_playTimer->stop();
        m_dfa_widget->setCorrectStatus();
    }
    showVariables();
    SetActInstruction();
}

FiniteAutomata FaToDFA::computeSolution()
{
    if(FA.hasEpsilon())
    {
        emit SendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return FiniteAutomata();
    }

    FiniteAutomata FAd;
    FAd.m_alphabet = FA.m_alphabet;                                 //FAd alphabet

    QSet< QSet<QString> > new_states;
    QSet<QString> act_state;

    FAd.m_startState = "{" + FA.m_startState + "}";                 //FAd start state
    QSet<QString> tmp_set; tmp_set.insert(FA.m_startState);
    new_states.insert(tmp_set);
    do
    {

        act_state=*(new_states.begin());
        QString from = qSetToQString(act_state);
        new_states.remove(act_state);
        FAd.m_states.insert(from);                                //insert new state to FAd
        foreach (QString a, FA.m_alphabet)
        {
            QSet<QString> discovered_state;
            foreach(ComputationalRules rule,FA.m_rules)
            {
                if(act_state.contains(rule.from)  && rule.symbol == a)
                {
                    discovered_state.insert(rule.to);
                }
            }


            QString to = qSetToQString(discovered_state);
            if (!discovered_state.empty())
            {
                FAd.m_rules.insert(ComputationalRules(from,to,a));    //FAd insert rules
                if(!FAd.m_states.contains(to))
                    new_states.insert(discovered_state);
                //FAd.states.insert(to);
            }
        }
        //new finite states
        if(!act_state.intersect(FA.m_finalStates).empty())
        {
            FAd.m_finalStates.insert(from);                           //insert final states to FAd
        }
    }while (!new_states.empty());

    return FAd;
}

void FaToDFA::CheckSolution()
{
    if(FA.hasEpsilon())
    {
        emit SendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return;
    }


    if(FiniteAutomata::AreEquivalent(DFA, m_correct_FA) && ! DFA.hasEpsilon() && DFA.isDeterministic())
    {
        m_dfa_widget->setCorrectStatus();
    }
    else
    {
        m_dfa_widget->setWrongStatus();
    }
}

void FaToDFA::ShowCorrectSolution()
{
    m_backup_FA = DFA;
    m_dfa_widget->setFA(new FiniteAutomata(m_correct_FA));
}

void FaToDFA::ShowUserSolution()
{
    m_dfa_widget->setFA(new FiniteAutomata(m_backup_FA));
}

void FaToDFA::ToBegin()
{
     m_algorithmWidget->enableNext();
     m_actPos=0;
     steps s = m_history.at(m_actPos);

     m_num = s.num ;
     m_actInstruction = s.actInstruction;
     m_prevInstruction = s.prewInstruction;
     DFA = s.DFA;
     m_dfa_widget->setFA(new FiniteAutomata(DFA));
     m_act_state = s.act_state;
     m_discovered_state = s.discovered_state;
     m_Q_new = s.Q_new;
     m_a = s.a;
     m_p = s.p;
     m_q = s.q;
     m_r = s.r;
     m_r_prime = s.r_prime;
     m_alphabet = s.alphabet;
     m_rules = s.rules;
     m_rules_prime = s.rules_prime;

     showVariables();
     SetActInstruction();
     m_dfa_widget->clearStatus();
     m_algorithmWidget->disablePrev();
}

void FaToDFA::ToEnd()
{
    while(m_actInstruction != lastInstruction)
        NextStep();
}

void FaToDFA::RemoveFuture()
{
    int count  = m_history.count();
    for(int i = m_actPos+1; i < count; i++)
    {
        m_history.removeLast();
    }
}

void FaToDFA::clearVariables()
{
    m_var_widget->Clear();
}

void FaToDFA::showVariables()
{
    int instruction;
    if(m_actInstruction == lastInstruction)
        instruction = m_prevInstruction;
    else
        instruction = m_actInstruction;

    m_var_widget->Clear();
    QString text;
    switch(instruction)
    {
        case HEADER:
            text = "";
            break;

        case VAR_INIT:
            text = varToString("s<sub>d</sub>",DFA.m_startState) + "<br>" ;                 // s_d =
            text += varToString("Q<sub>new</sub>", m_Q_new) + "<br>";
            text += varToString("R<sub>d</sub>","∅")+ "<br>";                                  // R_d = ∅
            text += varToString("Q<sub>d</sub>",DFA.m_states)+ "<br>";                                  // Q_d = ∅
            text += varToString("F<sub>d</sub>",DFA.m_finalStates);                                  // F_d = ∅
            break;
        case DO:
            text = "";
            break;
        case DO_INIT:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states);
            break;
        case FOREACH_A:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states)+ "<br>";
            text += varToString("a",m_a);
            break;
        case INIT_DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state);
            break;
        case FOREACH_RULE_IN_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state)+ "<br>";
            text += varToString("p",m_p)+ "<br>";
            text += varToString("q",m_q)+ "<br>";
            text += varToString("r",m_r.toString());
            break;
        case DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state)+ "<br>";
            text += varToString("q",m_q) + "<br>";
            text += varToString("r",m_r.toString());

            break;
        case IF_DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state) + "<br>";
            text += varToString("r",m_r.toString()) + "<br>";
            text += varToString("r'",m_r_prime.toString());
            break;
        case IF_Q_NEW:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state);
            break;
        case IF_FINAL:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.m_states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state) + "<br>";
            text += varToString("F",FA.m_finalStates) + "<br>";
            text += varToString("F<sub>d</sub>",DFA.m_finalStates);
            break;
        case WHILE_NEW:
            text = varToString("Q<sub>new</sub>",m_Q_new);
            break;

    }
    m_var_widget->SetText(text);
}

void FaToDFA::setInputFA(FiniteAutomata& _FA)
{
    m_not_dfa_widget->setFA(new FiniteAutomata(_FA));
}

void FaToDFA::setOutputFA(FiniteAutomata out_FA)
{
    m_dfa_widget->setFA(new FiniteAutomata(out_FA));
}

const FiniteAutomata &FaToDFA::GetInputFA()
{
    return *(m_not_dfa_widget->m_finiteAutomata);
}

const FiniteAutomata &FaToDFA::GetOutputFA()
{
    return *(m_dfa_widget->m_finiteAutomata);
}

void FaToDFA::saveStep()
{
    step s;

    s.num = ++m_num;
    s.actInstruction = m_actInstruction;
    s.prewInstruction = m_prevInstruction;
    s.DFA = DFA;
    s.act_state = m_act_state;
    s.discovered_state = m_discovered_state;
    s.Q_new = m_Q_new;
    s.a = m_a;
    s.p = m_p;
    s.q = m_q;
    s.r = m_r;
    s.r_prime = m_r_prime;
    s.alphabet = m_alphabet;
    s.rules = m_rules;
    s.rules_prime = m_rules_prime;

    m_history.append(s);
    m_actPos = m_history.count() - 1;
}

void FaToDFA::PrevStep()
{
    if (m_actPos > 0)
    {
        m_algorithmWidget->enableNext();
        m_actPos--;
        steps s = m_history.at(m_actPos);

        m_num = s.num ;
        m_actInstruction = s.actInstruction;
        m_prevInstruction = s.prewInstruction;
        DFA = s.DFA;
        m_dfa_widget->setFA(new FiniteAutomata(DFA));
        m_act_state = s.act_state;
        m_discovered_state = s.discovered_state;
        m_Q_new = s.Q_new;
        m_a = s.a;
        m_p = s.p;
        m_q = s.q;
        m_r = s.r;
        m_r_prime = s.r_prime;
        m_alphabet = s.alphabet;
        m_rules = s.rules;
        m_rules_prime = s.rules_prime;

        showVariables();
        SetActInstruction();
        m_dfa_widget->clearStatus();

    }
    else
    {
        m_algorithmWidget->disablePrev();
    }
}
