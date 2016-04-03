#include "pushdownautomata.h"

CPushDownAutomata::CPushDownAutomata()
{
}

void CPushDownAutomata::SetStackAlphabet(QSet<QString> stackAplhabet)
{
    m_stackAlphabet = stackAplhabet;
}

void CPushDownAutomata::AddPDARule(QString from, QString to, QString pushSymbol, QString popSymol, QString symbol)
{
    CPDACompotutationalRule newPDARule(from, to, pushSymbol, popSymol, symbol);
    m_pdaRules.insert(newPDARule);
}
