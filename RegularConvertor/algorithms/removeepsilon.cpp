#include "removeepsilon.h"

RemoveEpsilon::RemoveEpsilon(FiniteAutomata _FA)
{
    this->FA = _FA;
}

FiniteAutomata RemoveEpsilon::removeEpsilon()
{
    FiniteAutomata FA_not_epsilon;
    FA_not_epsilon.states = FA.states;
    FA_not_epsilon.alphabet = FA.alphabet;
    FA_not_epsilon.startState = FA.startState;

    //Rules
    foreach(QString state,FA.states)
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
    foreach(QString state,FA.states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1, epsilon_closer)
        {
            if(FA.finalStates.contains(state1))
            {
                FA_not_epsilon.addFinalState(state);
            }
        }
    }
    return FA_not_epsilon;
}
