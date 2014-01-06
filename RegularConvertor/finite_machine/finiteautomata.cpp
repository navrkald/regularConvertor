#include "finiteautomata.h"

FiniteAutomata::FiniteAutomata(QObject *parent) //:
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
    QString newStartState =  this->createUniqueName();
    QString newEndState = this->createUniqueName();
    //stavy
    this->addState(newStartState);
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
    this->addFinalState(newStartState);
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

    FiniteAutomata FA2_uniq_states = FA2;

    //prejmenovani stavu
    QSet <QString> same_states = FA1.states & FA2.states; //prunik názvů stavů
    if(same_states.size() != 0)
    {
        foreach (QString str, same_states)
        {
            FA2_uniq_states.renameState(str,str + "'");
        }
    }

    //stavy
    FA.states = FA1.states + FA2.states + FA2_uniq_states.states;  //sjednocení názvů stavů stavů

    // abeceda
    FA.alphabet = FA1.alphabet + FA2_uniq_states.alphabet;

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
        qDebug()<<"Mnozina pravidel uz obsahuje "<<rule.from<<" " <<rule.symbol<<"->"<<rule.to;
        return false;
    }
    rules.insert(rule);
    return true;
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


FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2)
{
    FiniteAutomata FA;

    //stavy
    FA.states = FA1.states + FA2.states;
    QString newStartState = FA.createUniqueName();
    FA.states.insert(newStartState);
    QString newEndState = FA.createUniqueName();
    FA.states.insert(newEndState);

    // abeceda
    FA.alphabet = FA1.alphabet + FA2.alphabet;

    //koncove stavy
    FA.finalStates.insert(newEndState);

    //Pocatecni stav
    FA.startState = newStartState;

    //pravidla
    FA.rules = FA1.rules + FA2.rules;
    FA.addRule(ComputationalRules(newStartState,FA1.startState,EPSILON));
    FA.addRule(ComputationalRules(newStartState,FA2.startState,EPSILON));
    foreach(QString end_state ,FA1.finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }
    foreach(QString end_state, FA2.finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }

    return FA;

}
