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

FaToDFA::FaToDFA(modes _mode, AlgorithmWidget* _algorithm_widget, FA_widget* _not_dfa_widget, FA_widget* _dfa_widget, QLabel* _var_widget, QObject* parrent)
 : Algorithm(parrent),  mode(_mode),  algorithm_widget(_algorithm_widget), not_dfa_widget(_not_dfa_widget), dfa_widget(_dfa_widget), var_widget(_var_widget)
{
    actInstruction = HEADER;
    prewInstruction = HEADER;
    instruction_count = WHILE_NEW+1;
    initInstructions();
    initBreakpoints(instruction_count);

    this->setColumnCount(1);
    this->setRowCount(instructions.count());

    var_widget->setText("");

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
    connect(this->algorithm_widget,SIGNAL(playPressed(int)),this,SLOT(runAlgorithm(int)));
    connect(this->algorithm_widget,SIGNAL(stopPressed()),this,SLOT(stop()));
    connect(this->algorithm_widget,SIGNAL(prewPressed()),this,SLOT(prevStep()));
    connect(this->algorithm_widget,SIGNAL(nextPressed()),this,SLOT(nextStep()));
    connect(this->algorithm_widget, SIGNAL(checkSolutionPressed()), this, SLOT(checkSolution()));
    connect(this->algorithm_widget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(showCorrectSolution()));
    connect(this->algorithm_widget, SIGNAL(showUserSolutionPressed()), this, SLOT(showUserSolution()));


    //
    // Connect timers.
    //
    connect(play_timer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(check_step_timer, SIGNAL(timeout()), this, SLOT(checkSolution()));

    // Connect Finite Automata widgets
    connect(not_dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setFA(FiniteAutomata*)));
    connect(dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));

    not_dfa_widget->setFA(new FiniteAutomata());

    algorithm_widget->enableShowButton();
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
    not_dfa_widget->clearStatus();
    setMode(mode);
}

void FaToDFA::setMode(Algorithm::modes _mode)
{
    mode = _mode;
    play_timer->stop();
    num = 0;
    actInstruction=HEADER;
    prewInstruction=HEADER;

    clearActInstruction();
    clearVariables();
    // because show correct solution break connectiom betwen user FA and user FA widget
    connect(dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));

    switch (mode)
    {
        case PLAY_MODE:
            algorithm_widget->enableNext();
            algorithm_widget->disablePrev();
            dfa_widget->setFA(new FiniteAutomata());
            //unselect instruction from algorithm window
            clearActInstruction();
            history.clear();
            actPos = 0;
            actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            saveStep();
        break;
        case CHECK_MODE: case STEP_MODE:
            correct_FA = computeSolution();
        break;
        case NONE:
        break;
    }

    if(mode == STEP_MODE)
        check_step_timer->start(CHECK_STEP_TIMEOUT);
    else
        check_step_timer->stop();

    dfa_widget->clearStatus();
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

    algorithm_widget->enablePrev();
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
            alphabet = FA.alphabet.toList();
            alphabet.sort();
            if( ! alphabet.empty())
                actInstruction = FOREACH_A;
            else
                actInstruction = IF_FINAL;
            break;
        case FOREACH_A:
            actInstruction = INIT_DOUBLE_PRIME_Q;
        break;
        case INIT_DOUBLE_PRIME_Q:
            rules = QSet<ComputationalRules>();
            foreach(QString from,act_state)
            {
                rules += FA.findRule_FromSymbol(from,a).toSet();
            }
            if( ! rules.empty())
                actInstruction = FOREACH_RULE_IN_Q;
            else
                actInstruction = IF_Q_NEW;
        break;
        case FOREACH_RULE_IN_Q:
            actInstruction = DOUBLE_PRIME_Q;
        break;
        case DOUBLE_PRIME_Q:
            if( ! rules.empty())
                actInstruction = FOREACH_RULE_IN_Q;
            else
                actInstruction = IF_Q_NEW;
        break;
        case IF_Q_NEW:
            actInstruction = IF_DOUBLE_PRIME_Q;
        break;
        case IF_DOUBLE_PRIME_Q:
            if ( ! alphabet.empty())
                actInstruction = FOREACH_A;
            else
                actInstruction = IF_FINAL;
        break;
        case IF_FINAL:
            actInstruction = WHILE_NEW;
        break;
        case WHILE_NEW:
            if ( ! Q_new.empty())
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
                dfa_widget->setFA(new FiniteAutomata(DFA));         //display start state
                QSet<QString> tmp_set; tmp_set.insert(FA.startState);
                Q_new.insert(tmp_set);
            }
            break;
        case DO:
            ;
            break;
        case DO_INIT:
            {
                act_state = *(Q_new.begin());
                Q_new.remove(act_state);
                act_state_name = qsetToQstring(act_state);
            }
            break;
        case FOREACH_A:
            a = alphabet.first();
            alphabet.pop_front();
            break;
        case INIT_DOUBLE_PRIME_Q:
            {
                discovered_state = QSet<QString>();

            }
            break;
        case FOREACH_RULE_IN_Q:
            r = *(rules.begin());
            rules.remove(r);
            p = r.from;
            q = r.to;
        break;
        case DOUBLE_PRIME_Q:
            discovered_state.insert(r.to);
        break;
        case IF_Q_NEW:
        {
            QString discovered_state_name = qsetToQstring(discovered_state);
            if( ! DFA.states.contains(QSet<QString>() << discovered_state_name) && ! discovered_state.empty())
            {
                Q_new.insert(discovered_state);
                DFA.states.insert(discovered_state_name);
                dfa_widget->addNodes(QSet<QString>() << discovered_state_name);
            }
        }
        break;
        case IF_DOUBLE_PRIME_Q:
            if( ! discovered_state.empty())
            {
                r_prime = ComputationalRules(qsetToQstring(act_state),qsetToQstring(discovered_state),a);
                DFA.rules.insert(r_prime);
                emit dfa_widget->addEdges(QSet<ComputationalRules>() << r_prime);
            }
        break;
        case IF_FINAL:
            if ( ! (act_state & FA.finalStates).empty())
            {
                DFA.finalStates.insert(qsetToQstring(act_state));
                dfa_widget->addEndingNodes(QSet<QString>() << qsetToQstring(act_state));
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
        algorithm_widget->disableNext();
        play_timer->stop();
        dfa_widget->setCorrectStatus();
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
        QString from = qsetToQstring(act_state);
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


            QString to = qsetToQstring(discovered_state);
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


    if(FiniteAutomata::areEquivalent(DFA, correct_FA) && ! DFA.hasEpsilon() && DFA.isDeterministic())
    {
        dfa_widget->setCorrectStatus();
    }
    else
    {
        dfa_widget->setWrongStatus();
    }
}

void FaToDFA::showCorrectSolution()
{
    disconnect(dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));
    dfa_widget->setFA(new FiniteAutomata(correct_FA));
}

void FaToDFA::showUserSolution()
{
    dfa_widget->setFA(new FiniteAutomata(DFA));
    connect(dfa_widget,SIGNAL(FA_changed(FiniteAutomata*)),this,SLOT(setDFA(FiniteAutomata*)));
}

void FaToDFA::removeFuture()
{
    int count  = history.count();
    for(int i = actPos+1; i < count; i++)
    {
        history.removeLast();
    }
}

void FaToDFA::clearVariables()
{
    var_widget->clear();
}

void FaToDFA::showVariables()
{
    int instruction;
    if(actInstruction == last_instruction)
        instruction = prewInstruction;
    else
        instruction = actInstruction;

    var_widget->clear();
    QString text;
    switch(instruction)
    {
        case HEADER:
            text = "";
            break;

        case VAR_INIT:
            text = varToString("s<sub>d</sub>",DFA.startState) + "<br>" ;                 // s_d =
            text += varToString("Q<sub>new</sub>", Q_new) + "<br>";
            text += varToString("R<sub>d</sub>","∅")+ "<br>";                                  // R_d = ∅
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";                                  // Q_d = ∅
            text += varToString("F<sub>d</sub>",DFA.finalStates);                                  // F_d = ∅
            break;
        case DO:
            text = "";
            break;
        case DO_INIT:
            text = varToString("Q'",act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states);
            break;
        case FOREACH_A:
            text = varToString("Q'",act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",a);
            break;
        case INIT_DOUBLE_PRIME_Q:
            text = varToString("Q'",act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",a)+ "<br>";
            text += varToString("Q''",discovered_state);
            break;
        case FOREACH_RULE_IN_Q:
            text = varToString("Q'",act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",a)+ "<br>";
            text += varToString("Q''",discovered_state)+ "<br>";
            text += varToString("p",p)+ "<br>";
            text += varToString("q",q)+ "<br>";
            text += varToString("r",r.toString());
            break;
        case DOUBLE_PRIME_Q:
            text = varToString("Q'",act_state)+ "<br>";
            text += varToString("Q<sub>new</sub>",Q_new)+ "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states)+ "<br>";
            text += varToString("a",a)+ "<br>";
            text += varToString("Q''",discovered_state)+ "<br>";
            text += varToString("q",q) + "<br>";
            text += varToString("r",r.toString());

            break;
        case IF_DOUBLE_PRIME_Q:
            text = varToString("Q'",act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",a) + "<br>";
            text += varToString("Q''",discovered_state) + "<br>";
            text += varToString("r",r.toString()) + "<br>";
            text += varToString("r'",r_prime.toString());
            break;
        case IF_Q_NEW:
            text = varToString("Q'",act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",a) + "<br>";
            text += varToString("Q''",discovered_state);
            break;
        case IF_FINAL:
            text = varToString("Q'",act_state) + "<br>";
            text += varToString("Q<sub>new</sub>",Q_new) + "<br>";
            text += varToString("Q<sub>d</sub>",DFA.states) + "<br>";
            text += varToString("a",a) + "<br>";
            text += varToString("Q''",discovered_state) + "<br>";
            text += varToString("F",FA.finalStates) + "<br>";
            text += varToString("F<sub>d</sub>",DFA.finalStates);
            break;
        case WHILE_NEW:
            text = varToString("Q<sub>new</sub>",Q_new);
            break;

    }
    var_widget->setText(text);
}

void FaToDFA::setInputFA(FiniteAutomata _FA)
{
    not_dfa_widget->setFA(new FiniteAutomata(_FA));
}

void FaToDFA::setOutputFA(FiniteAutomata out_FA)
{
    dfa_widget->setFA(new FiniteAutomata(out_FA));
}

void FaToDFA::saveStep()
{
    step s;

    s.num = ++num;
    s.actInstruction = actInstruction;
    s.prewInstruction = prewInstruction;
    s.DFA = DFA;
    s.act_state = act_state;
    s.discovered_state = discovered_state;
    s.Q_new = Q_new;
    s.a = a;
    s.p = p;
    s.q = q;
    s.r = r;
    s.r_prime = r_prime;
    s.alphabet = alphabet;
    s.rules = rules;
    s.rules_prime = rules_prime;

    history.append(s);
    actPos = history.count() - 1;
}

void FaToDFA::prevStep()
{
    if (actPos > 0)
    {
        algorithm_widget->enableNext();
        actPos--;
        steps s = history.at(actPos);

        num = s.num ;
        actInstruction = s.actInstruction;
        prewInstruction = s.prewInstruction;
        DFA = s.DFA;
        dfa_widget->setFA(new FiniteAutomata(DFA));
        act_state = s.act_state;
        discovered_state = s.discovered_state;
        Q_new = s.Q_new;
        a = s.a;
        p = s.p;
        q = s.q;
        r = s.r;
        r_prime = s.r_prime;
        alphabet = s.alphabet;
        rules = s.rules;
        rules_prime = s.rules_prime;

        showVariables();
        setActInstruction();
        dfa_widget->clearStatus();

    }
    else
    {
        algorithm_widget->disablePrev();
    }
}
