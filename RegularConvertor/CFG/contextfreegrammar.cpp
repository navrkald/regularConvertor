#include "contextfreegrammar.h"

CContextFreeGrammar::CContextFreeGrammar()
{
}

QSet<QString> CContextFreeGrammar::GetTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CNonTerminal& terminal, terminalsAlphabet)
    {
        outSet.insert(terminal.symbol);
    }
    return outSet;
}

QSet<QString> CContextFreeGrammar::GetNoonTerminalAlphabet()
{
    QSet<QString> outSet;
    foreach(CNonTerminal& nonTerminal, nonTerminalsAlphabet)
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
