#include "finiteautomata.h"

FiniteAutomata::FiniteAutomata(QObject *parent) :
    QObject(parent)
{
    nextId = 0;
    starState = "";
}

QString FiniteAutomata::createUniqueName()
{
    while(!isStateUnique(QString::number(nextId)))
       nextId++;
    return QString::number(nextId);
}

bool FiniteAutomata::isStateUnique(QString state)
{
    //debug
    if(states.contains(state))
        qDebug() << "Mnozina stavu uz obsahje \"" + state + "\".";
    else
        qDebug() << "Mnozina stavu uz nebsahje \"" + state + "\".";

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
    if(!isStateUnique(newStateName))
        return false;

    //rename start state
    if(starState == oldStateName)
        starState = newStateName;

    //rename state in finalStates
    if(finalStates.contains(oldStateName))
    {
        removeFinalState(oldStateName);
        addFinalState(newStateName);
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
            removeRule(rule);
            addRule(newRule);
        }
    }
    //rename state in states set
    removeState(oldStateName);
    return addState(newStateName);
}

void FiniteAutomata::changeStartState(QString StateName)
{
    this->starState = StateName;
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
