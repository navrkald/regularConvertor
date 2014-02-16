#include "dfatominfa.h"

DFAtoMinFA::DFAtoMinFA(FiniteAutomata _FA) : FA(_FA)
{
}


bool DFAtoMinFA::canDivide(QString symbol, QSet< QSet <QString> > Qm, QSet<QString> X, QSet <QString> &X1, QSet <QString> &X2)
{
    QSet <QString> Q1, Q2;
    QList<ComputationalRules> rules;

    //add all rules with from = X and witch symbol
    foreach(QString from,X)
    {
        rules.append(FA.findRule_FromSymbol(from,symbol));
    }

    //come throw all rules
    foreach (ComputationalRules rule,rules)
    {
        if(!X1.empty())
        {
            if (Q1 == *findInSubsets(Qm,rule.to).begin())
            {
                X1.insert(rule.from);
            }
            else if (Q2.empty())
            {
                Q2 = *findInSubsets(Qm,rule.to).begin();
                X2.insert(rule.from);
            }
            else if (Q2 == *findInSubsets(Qm,rule.to).begin())
            {
                X2.insert(rule.from);
            }
            else
            {
                qDebug() << "Fatal error when minimalise FA!";
                qDebug() << "Error occured in function fission()!";
                exit(EXIT_FAILURE);
            }
        }
        else //for first create new Q1
        {
            Q1 = *findInSubsets(Qm,rule.to).begin();
            X1.insert(rule.from);
        }
    }

    if (X2.empty())
        return false;
    else
        return true;
}

FiniteAutomata DFAtoMinFA::convert()
{
    this->FA.makeWellDefined();

    QSet< QSet <QString> > Qm;
    Qm.insert(FA.finalStates);
    Qm.insert(FA.states - FA.finalStates);
    bool divided = false;
    do
    {
        foreach(QSet<QString> X, Qm)
        {
            foreach(QString symbol,FA.alphabet)
            {
                QSet <QString> X1;
                QSet <QString> X2;
                if((divided = canDivide(symbol, Qm, X, X1, X2)) == true)
                {// Do fision
                    Qm.remove(X);
                    Qm.insert(X1);
                    Qm.insert(X2);
                }

            }
        }
    }while(divided);

    //Lets create new minFA
    FiniteAutomata minFA;
    minFA.alphabet = FA.alphabet;
    minFA.states = setOfSubsetsToSet(Qm);
    QSet<QString> gg =  *findInSubsets(Qm, FA.startState).begin();
    minFA.startState = qsetToQstring(gg);
    minFA.finalStates = setOfSubsetsToSet(findInSubsets(Qm, FA.finalStates));
    //Add new rules
    foreach(QSet <QString> from_set,Qm)
    {
        foreach(QString symbol, FA.alphabet)
        {
            QList <ComputationalRules> rules =  FA.findRule_FromSymbol(from_set,symbol);
            minFA.addRule(qsetToQstring(getFroms(rules)), qsetToQstring(getTos(rules)),symbol);
        }
    }

    return minFA;

}
