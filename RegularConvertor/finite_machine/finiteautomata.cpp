#include "finiteautomata.h"

FiniteAutomata::FiniteAutomata(QObject *parent) :
    QObject(parent)
{
    nextId = 0;
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

}

void FiniteAutomata::renameState(QString oldStateName, QString newStateName)
{

}

void FiniteAutomata::changeStartState(QString StateName)
{
}

void FiniteAutomata::addFinalState(QString StateName)
{
}

void FiniteAutomata::removeFinalState(QString StateName)
{
}

void FiniteAutomata::addSymbol(QString symbol)
{
}

void FiniteAutomata::removeSymbol(QString symbol)
{
}

void FiniteAutomata::addRule()
{
}

void FiniteAutomata::removeRule()
{
}

void FiniteAutomata::changeRule()
{
}
