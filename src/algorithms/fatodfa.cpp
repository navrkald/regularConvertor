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

FaToDFA::FaToDFA(FiniteAutomata _FA) : FA(_FA)
{
    ;
}

FaToDFA::~FaToDFA()
{

}

FaToDFA::FaToDFA(modes _mode, CAlgorithmWidget* _algorithm_widget, FA_widget* _not_dfa_widget, FA_widget* _dfa_widget, QLabel* _var_widget, QObject* parrent)
 : Algorithm(parrent)
{
    Init(_mode, _algorithm_widget, _not_dfa_widget, _dfa_widget, _var_widget, parrent);
}

void FaToDFA::Init(Algorithm::modes _mode, CAlgorithmWidget *_algorithm_widget, FA_widget *_not_dfa_widget, FA_widget *_dfa_widget, QLabel *_var_widget, QObject *parrent)
{
    m_mode = _mode;
    m_algorithm_widget = _algorithm_widget;
    m_not_dfa_widget = _not_dfa_widget;
    m_dfa_widget = _dfa_widget;
    m_var_widget = _var_widget;

    actInstruction = HEADER;
    prewInstruction = HEADER;
    instruction_count = WHILE_NEW+1;
    initInstructions();
    initBreakpoints(instruction_count);

    this->setColumnCount(1);
    this->setRowCount(instructions.count());

    m_var_widget->setText("");

    for(int i = 0; i < instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,instructions[i],Qt::EditRole);
        setData(index,true,Algorithm::HasBreakpoint_Role);
        setData(index,false,Algorithm::Breakpoint_Role);
        switch(i)
        {
            case HEADER:
                setData(index,false,Algorithm::HasBreakpoint_Role);
                break;
        }
    }


    //
    // Connect algorithm buttons.
    //
    connect(this->m_algorithm_widget,SIGNAL(playPressed(int)),this,SLOT(runAlgorithm(int)));
    connect(this->m_algorithm_widget,SIGNAL(stopPressed()),this,SLOT(stop()));
    connect(this->m_algorithm_widget,SIGNAL(prewPressed()),this,SLOT(prevStep()));
    connect(this->m_algorithm_widget,SIGNAL(nextPressed()),this,SLOT(nextStep()));
    connect(this->m_algorithm_widget, SIGNAL(checkSolutionPressed()), this, SLOT(checkSolution()));
    connect(this->m_algorithm_widget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(showCorrectSolution()));
    connect(this->m_algorithm_widget, SIGNAL(showUserSolutionPressed()), this, SLOT(showUserSolution()));
    connect(this->m_algorithm_widget, SIGNAL(beginPressed()), this, SLOT(toBegin()));
    connect(this->m_algorithm_widget, SIGNAL(endPressed()), this, SLOT(toEnd()));

    //
    // Connect timers.
    //
    connect(play_timer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(check_step_timer, SIGNAL(timeout()), this, SLOT(checkSolution()));

    // Connect Finite Automata widgets
    connect(m_not_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setFA(FiniteAutomata*)));
    connect(m_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));

    m_not_dfa_widget->setFA(new FiniteAutomata());

    m_algorithm_widget->enableShowButton();
}



void FaToDFA::initInstructions()
{
    instructions.resize(instruction_count);
    instructions[HEADER] =                  tr("<b>Input:</b> FA without ε-rules <i>M</i>=(Q, Σ, R, s, F)<br><b>Output:</b> DKA M'=(Q<sub>d</sub>, Σ, R<sub>d</sub>, s<sub>d</sub>, F<sub>d</sub>)");
    instructions[VAR_INIT] =                tr("s<sub>d</sub> = {s}; Q<sub>new</sub> = {s<sub>d</sub>}; R<sub>d</sub> = ∅; Q<sub>d</sub> = Q<sub>new</sub>; F<sub>d</sub> = ∅");
    instructions[DO] =                      tr("<b>do</b>");
    instructions[DO_INIT] =                 INDENT     + tr("Q' ∈ Q<sub>new</sub>; Q<sub>new</sub> = Q<sub>new</sub> - {Q'};");
    instructions[FOREACH_A] =               INDENT     + tr("<b>for each</b> a ∈ Σ <b>do</b>");
    instructions[INIT_DOUBLE_PRIME_Q]=      INDENT  INDENT      + tr("Q'' = ∅");
    instructions[FOREACH_RULE_IN_Q] =       INDENT  INDENT     + tr("<b>for each</b> r = {p a → q} <b>where</b> p ∈ Q'");
    instructions[DOUBLE_PRIME_Q] =          INDENT  INDENT  INDENT     + tr("Q'' ∈ Q'' ∪ {q}");
    instructions[IF_Q_NEW] =                INDENT  INDENT     + tr("<b>if</b> Q'' ∉ Q<sub>d</sub> ∪ {∅} <b>then</b> Q<sub>new</sub> = Q<sub>new</sub> ∪ {Q''}; Q<sub>d</sub> = Q<sub>d</sub> ∪ {Q''};");
    instructions[IF_DOUBLE_PRIME_Q] =       INDENT  INDENT     + tr("<b>if</b> Q'' ≠ ∅ <b>then</b> R<sub>d</sub> = R<sub>d</sub> ∪ r'={Q' a → Q''};");
    instructions[IF_FINAL] =                INDENT      + tr("<b>if</b> Q' ∩ F ≠ ∅ <b>then</b> F<sub>d</sub> = F<sub>d</sub> ∪ {Q'}");
    instructions[WHILE_NEW] =               tr("<b>until</b> Q<sub>new</sub> = ∅");
}

void FaToDFA::setFA(FiniteAutomata *_FA)
{
    this->FA = *_FA;
    if(this->FA.hasEpsilon())
        emit sendStatusBarMessage(tr("WARNING: Input finite automata has epsilon rules."));
    m_not_dfa_widget->clearStatus();
    setMode(m_mode);
}

void FaToDFA::setMode(Algorithm::modes _mode)
{
    m_mode = _mode;
    play_timer->stop();
    num = 0;
    actInstruction=HEADER;
    prewInstruction=HEADER;

    clearActInstruction();
    clearVariables();
    // because show correct solution break connectiom betwen user FA and user FA widget
    connect(m_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));

    switch (m_mode)
    {
        case PLAY_MODE:
            m_algorithm_widget->enableNext();
            m_algorithm_widget->disablePrev();
            m_dfa_widget->setFA(new FiniteAutomata());
            //unselect instruction from algorithm window
            clearActInstruction();
            m_history.clear();
            actPos = 0;
            actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            saveStep();
        break;
        case CHECK_MODE: case STEP_MODE:
            m_correct_FA = computeSolution();
        break;
        case NONE:
        break;
    }

    if(m_mode == STEP_MODE)
        check_step_timer->start(CHECK_STEP_TIMEOUT);
    else
        check_step_timer->stop();

    m_dfa_widget->clearStatus();
}

void FaToDFA::setDFA(FiniteAutomata *_FA)
{
    this->DFA = *_FA;
}



void FaToDFA::nextStep()
{
    if(FA.startState == "")
    {
        emit sendStatusBarMessage("WARNING: Input FA has not start state.");
        return;
    }
    if(FA.hasEpsilon())
    {
        emit sendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return;
    }

    m_algorithm_widget->enablePrev();
    switch(prewInstruction)
    {
        case HEADER:
            actInstruction = VAR_INIT;
        break;
        case VAR_INIT:
            actInstruction = DO;
        break;
        case DO:
            actInstruction = DO_INIT;
        break;
        case DO_INIT:
            m_alphabet = FA.alphabet.toList();
            m_alphabet.sort();
            if( ! m_alphabet.empty())
                actInstruction = FOREACH_A;
            else
                actInstruction = IF_FINAL;
            break;
        case FOREACH_A:
            actInstruction = INIT_DOUBLE_PRIME_Q;
        break;
        case INIT_DOUBLE_PRIME_Q:
            m_rules = QSet<ComputationalRules>();
            foreach(QString from,m_act_state)
            {
                m_rules += FA.findRule_FromSymbol(from,m_a).toSet();
            }
            if( ! m_rules.empty())
                actInstruction = FOREACH_RULE_IN_Q;
            else
                actInstruction = IF_Q_NEW;
        break;
        case FOREACH_RULE_IN_Q:
            actInstruction = DOUBLE_PRIME_Q;
        break;
        case DOUBLE_PRIME_Q:
            if( ! m_rules.empty())
                actInstruction = FOREACH_RULE_IN_Q;
            else
                actInstruction = IF_Q_NEW;
        break;
        case IF_Q_NEW:
            actInstruction = IF_DOUBLE_PRIME_Q;
        break;
        case IF_DOUBLE_PRIME_Q:
            if ( ! m_alphabet.empty())
                actInstruction = FOREACH_A;
            else
                actInstruction = IF_FINAL;
        break;
        case IF_FINAL:
            actInstruction = WHILE_NEW;
        break;
        case WHILE_NEW:
            if ( ! m_Q_new.empty())
                actInstruction = DO;
            else
                actInstruction = last_instruction;
        break;
    }

    switch(actInstruction)
    {
        case VAR_INIT:
            {
                DFA.startState = "{" + FA.startState + "}";
                DFA.alphabet = FA.alphabet;
                DFA.states.insert(DFA.startState);
                m_dfa_widget->setFA(new FiniteAutomata(DFA));         //display start state
                QSet<QString> tmp_set; tmp_set.insert(FA.startState);
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
            if( ! DFA.states.contains(QSet<QString>() << discovered_state_name) && ! m_discovered_state.empty())
            {
                m_Q_new.insert(m_discovered_state);
                DFA.states.insert(discovered_state_name);
                m_dfa_widget->addNodes(QSet<QString>() << discovered_state_name);
            }
        }
        break;
        case IF_DOUBLE_PRIME_Q:
            if( ! m_discovered_state.empty())
            {
                m_r_prime = ComputationalRules(qSetToQString(m_act_state),qSetToQString(m_discovered_state),m_a);
                DFA.rules.insert(m_r_prime);
                emit m_dfa_widget->addEdges(QSet<ComputationalRules>() << m_r_prime);
            }
        break;
        case IF_FINAL:
            if ( ! (m_act_state & FA.finalStates).empty())
            {
                DFA.finalStates.insert(qSetToQString(m_act_state));
                m_dfa_widget->addEndingNodes(QSet<QString>() << qSetToQString(m_act_state));
            }
        break;
        case WHILE_NEW:
            ;
        break;
    }

    removeFuture();
    if(actInstruction != last_instruction)
    {
        prewInstruction = actInstruction;
        saveStep();
        if(breakpoints[actInstruction])
            play_timer->stop();
    }
    else
    {
        m_algorithm_widget->disableNext();
        play_timer->stop();
        m_dfa_widget->setCorrectStatus();
    }
    showVariables();
    setActInstruction();
}

FiniteAutomata FaToDFA::computeSolution()
{
    if(FA.hasEpsilon())
    {
        emit sendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return FiniteAutomata();
    }

    FiniteAutomata FAd;
    FAd.alphabet = FA.alphabet;                                 //FAd alphabet

    QSet< QSet<QString> > new_states;
    QSet<QString> act_state;

    FAd.startState = "{" + FA.startState + "}";                 //FAd start state
    QSet<QString> tmp_set; tmp_set.insert(FA.startState);
    new_states.insert(tmp_set);
    do
    {

        act_state=*(new_states.begin());
        QString from = qSetToQString(act_state);
        new_states.remove(act_state);
        FAd.states.insert(from);                                //insert new state to FAd
        foreach (QString a, FA.alphabet)
        {
            QSet<QString> discovered_state;
            foreach(ComputationalRules rule,FA.rules)
            {
                if(act_state.contains(rule.from)  && rule.symbol == a)
                {
                    discovered_state.insert(rule.to);
                }
            }


            QString to = qSetToQString(discovered_state);
            if (!discovered_state.empty())
            {
                FAd.rules.insert(ComputationalRules(from,to,a));    //FAd insert rules
                if(!FAd.states.contains(to))
                    new_states.insert(discovered_state);
                //FAd.states.insert(to);
            }
        }
        //new finite states
        if(!act_state.intersect(FA.finalStates).empty())
        {
            FAd.finalStates.insert(from);                           //insert final states to FAd
        }
    }while (!new_states.empty());

    return FAd;
}

void FaToDFA::checkSolution()
{
    if(FA.hasEpsilon())
    {
        emit sendStatusBarMessage("ERROR: Input FA has epsilon rules.");
        return;
    }


    if(FiniteAutomata::areEquivalent(DFA, m_correct_FA) && ! DFA.hasEpsilon() && DFA.isDeterministic())
    {
        m_dfa_widget->setCorrectStatus();
    }
    else
    {
        m_dfa_widget->setWrongStatus();
    }
}

void FaToDFA::showCorrectSolution()
{
    m_backup_FA = DFA;
    m_dfa_widget->setFA(new FiniteAutomata(m_correct_FA));
}

void FaToDFA::showUserSolution()
{
    m_dfa_widget->setFA(new FiniteAutomata(m_backup_FA));
}

void FaToDFA::toBegin()
{
     m_algorithm_widget->enableNext();
     actPos=0;
     steps s = m_history.at(actPos);

     num = s.num ;
     actInstruction = s.actInstruction;
     prewInstruction = s.prewInstruction;
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
     setActInstruction();
     m_dfa_widget->clearStatus();
     m_algorithm_widget->disablePrev();
}

void FaToDFA::toEnd()
{
    while(actInstruction != last_instruction)
        nextStep();
}

void FaToDFA::removeFuture()
{
    int count  = m_history.count();
    for(int i = actPos+1; i < count; i++)
    {
        m_history.removeLast();
    }
}

void FaToDFA::clearVariables()
{
    m_var_widget->clear();
}

void FaToDFA::showVariables()
{
    int instruction;
    if(actInstruction == last_instruction)
        instruction = prewInstruction;
    else
        instruction = actInstruction;

    m_var_widget->clear();
    QString text;
    switch(instruction)
    {
        case HEADER:
            text = "";
            break;

        case VAR_INIT:
            text = varToString("s<sub>d</sub>",DFA.startState) + "<br>" ;                 // s_d =
            text += varToString("Q<sub>new</sub>", m_Q_new) + "<br>";
            text += varToString("R<sub>d</sub>","∅")+ "<br>";                                  // R_d = ∅
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";                                  // Q_d = ∅
            text += varToString("F<sub>d</sub>",DFA.finalStates);                                  // F_d = ∅
            break;
        case DO:
            text = "";
            break;
        case DO_INIT:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states);
            break;
        case FOREACH_A:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",m_a);
            break;
        case INIT_DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state);
            break;
        case FOREACH_RULE_IN_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state)+ "<br>";
            text += varToString("p",m_p)+ "<br>";
            text += varToString("q",m_q)+ "<br>";
            text += varToString("r",m_r.toString());
            break;
        case DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",m_a)+ "<br>";
            text += varToString("Q''",m_discovered_state)+ "<br>";
            text += varToString("q",m_q) + "<br>";
            text += varToString("r",m_r.toString());

            break;
        case IF_DOUBLE_PRIME_Q:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state) + "<br>";
            text += varToString("r",m_r.toString()) + "<br>";
            text += varToString("r'",m_r_prime.toString());
            break;
        case IF_Q_NEW:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state);
            break;
        case IF_FINAL:
            text = varToString("Q'",m_act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",m_Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",m_a) + "<br>";
            text += varToString("Q''",m_discovered_state) + "<br>";
            text += varToString("F",FA.finalStates) + "<br>";
            text += varToString("F<sub>d</sub>",DFA.finalStates);
            break;
        case WHILE_NEW:
            text = varToString("Q<sub>new</sub>",m_Q_new);
            break;

    }
    m_var_widget->setText(text);
}

void FaToDFA::setInputFA(FiniteAutomata _FA)
{
    m_not_dfa_widget->setFA(new FiniteAutomata(_FA));
}

void FaToDFA::setOutputFA(FiniteAutomata out_FA)
{
    m_dfa_widget->setFA(new FiniteAutomata(out_FA));
}

void FaToDFA::saveStep()
{
    step s;

    s.num = ++num;
    s.actInstruction = actInstruction;
    s.prewInstruction = prewInstruction;
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
    actPos = m_history.count() - 1;
}

void FaToDFA::prevStep()
{
    if (actPos > 0)
    {
        m_algorithm_widget->enableNext();
        actPos--;
        steps s = m_history.at(actPos);

        num = s.num ;
        actInstruction = s.actInstruction;
        prewInstruction = s.prewInstruction;
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
        setActInstruction();
        m_dfa_widget->clearStatus();

    }
    else
    {
        m_algorithm_widget->disablePrev();
    }
}
