#ifndef PUSHDOWNAUTOMATA_H
#define PUSHDOWNAUTOMATA_H

#include "finiteautomata.h"
#include "pdacompotutationalrules.h"
class CPushDownAutomata : public FiniteAutomata
{
public:
    CPushDownAutomata();
    void SetStackAlphabet(QSet<QString> stackAplhabet);
    void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QVector<QString> pushSymbols);
    void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QString pushSymbol);
public:
    QSet <QString> m_stackAlphabet;
    QSet <CPDACompotutationalRule> m_pdaRules;
};

#endif // PUSHDOWNAUTOMATA_H
