#include "finiteautomata.h"

//#define NEW_NUM_NAME
#define NEW_COMMA_NAME

FiniteAutomata::FiniteAutomata() //:
    //QObject(parent)
{
    nextId = 0;
    startState = "";
}

//create finite automata from primitive reg expression
FiniteAutomata::FiniteAutomata(QString symbol)
{
    init(symbol);
}

void FiniteAutomata::init(QString symbol)
{
    nextId = 0;

    //stavy
    QString newStartState =  this->createUniqueName();
    this->addState(newStartState);
    QString newEndState = this->createUniqueName();
    this->addState(newEndState);

    //symbol
    if(symbol != EPSILON)
    {
        this->addSymbol(symbol);
    }
    //pravidlo
    this->addRule(ComputationalRules(newStartState, newEndState, symbol));
    //startovni stav
    this->startState = newStartState;
    //koncovy stav
    this->addFinalState(newEndState);
}

QString FiniteAutomata::createUniqueName()
{
    //TODO overit zda v definici KA muze/nesmi byt abeceda a stavy disjunktni, pokud nesmi, pak udelat kontrolu zda novy unikatni stav nekoliduje s abecedou
    while(!isStateUnique(QString::number(nextId)))
       nextId++;
    return QString::number(nextId);
}


FiniteAutomata FiniteAutomata::concatenate(FiniteAutomata FA1, FiniteAutomata FA2)
{
    FiniteAutomata FA;

    //stavy
    FiniteAutomata FA2_uniq_states = FA2;

    FA.states = FA1.states + FA2.states;

    //prejmenovani stavu
    QSet <QString> same_states = FA1.states & FA2.states; //prunik názvů stavů
    if(same_states.size() != 0)
    {
        foreach (QString str, same_states)
        {

#ifdef NEW_NUM_NAME
            QString uniqName = FA.createUniqueName();
#endif

#ifdef NEW_COMMA_NAME
            QString uniqName = str;
            do
            {
                uniqName += "'";
            }
            while(!FA.isStateUnique(uniqName));
#endif

            FA2_uniq_states.renameState(str,uniqName);
            FA.addState(uniqName);
        }
    }



    // abeceda
    FA.alphabet = FA1.alphabet + FA2.alphabet;

    //koncove stavy
    FA.finalStates = FA2_uniq_states.finalStates;

    //Pocatecni stav
    FA.startState = FA1.startState;

    //pravidla
    FA.rules = FA1.rules + FA2_uniq_states.rules;
    foreach(QString end_state,FA1.finalStates)
    {
        ComputationalRules bridge(end_state, FA2_uniq_states.startState, EPSILON);
        FA.addRule(bridge);
    }

    return FA;
}

FiniteAutomata FiniteAutomata::iteration(FiniteAutomata FA1)
{
    FiniteAutomata FA;

    //stavy
    FA.states = FA1.states;
    QString newStartState = FA.createUniqueName();
    FA.states.insert(newStartState);
    QString newEndState = FA.createUniqueName();
    FA.states.insert(newEndState);

    // abeceda
    FA.alphabet = FA1.alphabet;

    //koncove stavy
    FA.finalStates.insert(newEndState);

    //Pocatecni stav
    FA.startState = newStartState;

    //pravidla
    FA.rules = FA1.rules ;
    FA.addRule(ComputationalRules(newStartState,FA1.startState,EPSILON)); //startovni stav
    foreach(QString end_state ,FA1.finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON)); //koncove stavy do noveho
        FA.addRule(ComputationalRules(end_state,FA1.startState,EPSILON)); //z puvodniho koncoveho stavu do puvodniho startovniho
    }
    FA.addRule(ComputationalRules(newStartState,newEndState,EPSILON)); //ze startovniho stavu do koncoveho


    return FA;
}

bool FiniteAutomata::isStateUnique(QString state)
{
//    //debug
//    if(states.contains(state))
//        qDebug() << "Mnozina stavu uz obsahje \"" + state + "\".";
//    else
//        qDebug() << "Mnozina stavu uz nebsahje \"" + state + "\".";

    return !states.contains(state);
}

//prida stav
//pokud je jmeno unikatni vrati true, jinak false
bool FiniteAutomata::addState(QString stateName)
{
    if(!isStateUnique(stateName))
        return false;

    states.insert(stateName);
    return true;

}


bool FiniteAutomata::removeState(QString stateName)
{
    return states.remove(stateName);
}

bool FiniteAutomata::renameState(QString oldStateName, QString newStateName)
{
    if(!this->isStateUnique(newStateName))
        return false;

    //rename start state
    if(startState == oldStateName)
        startState = newStateName;

    //rename state in finalStates
    if(finalStates.contains(oldStateName))
    {
        this->removeFinalState(oldStateName);
        this->addFinalState(newStateName);
    }

    //rename state name in all rules
    foreach(ComputationalRules rule,rules)
    {
        if(rule.from == oldStateName || rule.to == oldStateName)
        {
            ComputationalRules newRule = rule;
            if(rule.from == oldStateName)
                newRule.from = newStateName;
            if(rule.to == oldStateName)
                newRule.to = newStateName;
            this->removeRule(rule);
            this->addRule(newRule);
        }
    }
    //rename state in states set
    this->removeState(oldStateName);
    return addState(newStateName);
}

void FiniteAutomata::changeStartState(QString StateName)
{
    this->startState = StateName;
}

void FiniteAutomata::addFinalState(QString StateName)
{
    finalStates.insert(StateName);
}

void FiniteAutomata::removeFinalState(QString StateName)
{
    finalStates.remove(StateName);
}

void FiniteAutomata::addSymbol(QString symbol)
{
    alphabet.insert(symbol);
}

void FiniteAutomata::removeSymbol(QString symbol)
{
    alphabet.remove(symbol);
}

bool FiniteAutomata::addRule(ComputationalRules rule)
{
    if(rules.contains(rule))
    {
        //qDebug()<<"Mnozina pravidel uz obsahuje "<<rule.from<<" " <<rule.symbol<<"->"<<rule.to;
        return false;
    }
    rules.insert(rule);
    return true;
}

bool FiniteAutomata::addRule(QString from, QString to, QString symbol)
{
    ComputationalRules newRule(from,to,symbol);
    return addRule(newRule);
}

void FiniteAutomata::removeRule(ComputationalRules rule)
{
    rules.remove(rule);
}

bool FiniteAutomata::changeSymbolInRule(ComputationalRules rule, QString symbol)
{
    removeRule(rule);
    ComputationalRules newRule = rule;
    newRule.symbol = symbol;
    return addRule(newRule);
}


//vrati true kdyz jsou odlisne, jinak true
bool FiniteAutomata::changeRule(ComputationalRules oldrule, ComputationalRules newrule)
{
    if(oldrule == newrule)
    {
        return false;
    }
    else
    {
        rules.remove(oldrule);
        rules.insert(newrule);
        return true;
    }
}

QSet<QString> FiniteAutomata::epsilonCloser(QString state)
{
    //empty set
    QSet<QString> Q_act;
    if (!this->states.contains(state))
    {
        return Q_act;
    }

    //search epsilon closer
    Q_act.insert(state);
    QSet<QString> Q_prew;
    do
    {
        Q_prew = Q_act;
        foreach (QString state1, Q_act)
            Q_act += epsilonNeighbours(state1);
    }while(Q_act != Q_prew);
    return Q_act;
}

QSet<QString> FiniteAutomata::epsilonNeighbours(QString state)
{
    QSet<QString> epsilon_neighbours;
    if (!this->states.contains(state))
    {
        return epsilon_neighbours;
    }
    else
    {
        foreach(ComputationalRules rule,this->rules)
        {
            if(rule.from == state && rule.symbol == EPSILON)
                epsilon_neighbours.insert(rule.to);
        }
        return epsilon_neighbours;
    }

}

QSet<ComputationalRules> FiniteAutomata::nonEpsilonRulesOf(QString state)
{
    QSet<ComputationalRules> non_epsilon_rules;
    foreach(ComputationalRules rule, this->rules)
    {
        if(rule.from == state && rule.symbol != EPSILON)
        {
            non_epsilon_rules.insert(rule);
        }
    }
    return non_epsilon_rules;
}


FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2)
{
    FiniteAutomata FA;

    //stavy
    FA.states = FA1.states + FA2.states;

    //prejmenovani stavu
    FiniteAutomata FA2_uniq_states = FA2;
    QSet <QString> same_states = FA1.states & FA2.states; //prunik názvů stavů
    if(same_states.size() != 0)
    {
        foreach (QString str, same_states)
        {
#ifdef NEW_NUM_NAME
            QString uniqName = FA.createUniqueName();
#endif

#ifdef NEW_COMMA_NAME
            QString uniqName = str;
            do
            {
                uniqName += "'";
            }
            while(!FA.isStateUnique(uniqName));
#endif

            FA2_uniq_states.renameState(str,uniqName);
            FA.addState(uniqName);
        }
    }


    QString newStartState = FA.createUniqueName();
    FA.states.insert(newStartState);
    QString newEndState = FA.createUniqueName();
    FA.states.insert(newEndState);



    // abeceda
    FA.alphabet = FA1.alphabet + FA2_uniq_states.alphabet;

    //koncove stavy
    FA.finalStates.insert(newEndState);

    //Pocatecni stav
    FA.startState = newStartState;

    //pravidla
    FA.rules = FA1.rules + FA2_uniq_states.rules;
    FA.addRule(ComputationalRules(newStartState,FA1.startState,EPSILON));
    FA.addRule(ComputationalRules(newStartState,FA2_uniq_states.startState,EPSILON));
    foreach(QString end_state ,FA1.finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }
    foreach(QString end_state, FA2_uniq_states.finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }

    return FA;

}


bool operator ==(const FiniteAutomata FA1, const FiniteAutomata FA2)
{
    if (FA1.states != FA2.states)
        return false;

    if (FA1.alphabet != FA2.alphabet)
        return false;

    if (FA1.startState != FA2.startState)
        return false;

    if (FA1.finalStates != FA2.finalStates)
        return false;

    if (FA1.rules != FA2.rules)
        return false;

    return true;
}
