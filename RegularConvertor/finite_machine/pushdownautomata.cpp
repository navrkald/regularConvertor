#include "pushdownautomata.h"

CPushDownAutomata::CPushDownAutomata()
{
}

void CPushDownAutomata::SetStackAlphabet(QSet<QString> stackAplhabet)
{
    m_stackAlphabet = stackAplhabet;
}

void CPushDownAutomata::AddPDARule(QString from, QString to, QString symbol, QString popSymol, QVector<QString> pushSymbols)
{
    CPDACompotutationalRule newPDARule(from, to, symbol, popSymol, pushSymbols);
    m_pdaRules.insert(newPDARule);
}

void CPushDownAutomata::AddPDARule(QString from, QString to, QString symbol, QString popSymol, QString pushSymbol)
{
    CPDACompotutationalRule newPDARule(from, to, symbol, popSymol, pushSymbol);
    m_pdaRules.insert(newPDARule);
}
