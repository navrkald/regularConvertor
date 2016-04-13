#include "contextfreegrammar.h"

QVector<QString> CCFGRule::GetRevertedRightRule()
{
    QVector<QString> revertedVector;
    foreach(CSymbolPtr symbol, rightString)
    {
        QString symbol_str = symbol->m_symbol;
        revertedVector.push_front(symbol_str);
    }
    return revertedVector;
}

CContextFreeGrammar::CContextFreeGrammar()
{
}

QSet<QString> CContextFreeGrammar::GetTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CTerminal terminal, m_terminalsAlphabet)
    {
        outSet.insert(terminal.m_symbol);
    }
    return outSet;
}

QSet<QString> CContextFreeGrammar::GetNoonTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CNonTerminal nonTerminal, m_nonterminalsAlphabet)
    {
        outSet.insert(nonTerminal.m_symbol);
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

int CContextFreeGrammar::GetRulesCount()
{
    return m_rules.size();
}
