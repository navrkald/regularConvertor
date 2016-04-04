#include "contextfreegrammar.h"

CContextFreeGrammar::CContextFreeGrammar()
{
}

QSet<QString> CContextFreeGrammar::GetTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CTerminal terminal, m_terminalsAlphabet)
    {
        outSet.insert(terminal.symbol);
    }
    return outSet;
}

QSet<QString> CContextFreeGrammar::GetNoonTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CNonTerminal nonTerminal, m_nonterminalsAlphabet)
    {
        outSet.insert(nonTerminal.symbol);
    }
    return outSet;
}

QSet<QString> CContextFreeGrammar::GetBothTerminalAndNonterminalAlphabet()
{
    QSet<QString> terninals = GetTerminalAlphabet();
    QSet<QString> nonTerminals = GetNoonTerminalAlphabet();
    QSet<QString> outSet = terninals + nonTerminals;
    return outSet;
}
