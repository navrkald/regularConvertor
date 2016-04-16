#include "contextfreegrammar.h"

QVector<QString> CCFGRule::GetRevertedRightRule()
{
    QVector<QString> revertedVector;
    foreach(CSymbol symbol, m_rightString)
    {
        QString symbol_str = symbol.GetString();
        revertedVector.push_front(symbol_str);
    }
    return revertedVector;
}

CContextFreeGrammar::CContextFreeGrammar()
{
}

//QSet<CTerminal> CContextFreeGrammar::GetTerminalAlphabet()
//{
//    QSet<QString> outSet;
//    foreach(CTerminal terminal, m_terminalsAlphabet)
//    {
//        outSet.insert(terminal.GetString());
//    }
//    return outSet;
//}

//QSet<CNonTerminal> CContextFreeGrammar::GetNoonTerminalAlphabet()
//{
//    QSet<QString> outSet;
//    foreach(CNonTerminal nonTerminal, m_nonterminalsAlphabet)
//    {
//        outSet.insert(nonTerminal.GetString());
//    }
//    return outSet;
//}

QSet<QString> CContextFreeGrammar::GetBothTerminalAndNonterminalAlphabet()
{
    QSet<QString> terninals = CTerminal::CTerminalQSetToQStringQSet(GetTerminalAlphabet());
    QSet<QString> nonTerminals = CNonTerminal::CNonTerminalQSetToQStringQSet(GetNoonTerminalAlphabet());
    QSet<QString> outSet = terninals + nonTerminals;
    return outSet;
}

int CContextFreeGrammar::GetRulesCount()
{
    return m_rules.size();
}

QSet<QString> CSymbol::CSymbolQSetToQStringQSet(const QSet<CSymbol> &csymbolSet)
{
  QSet<QString> outSet;
  foreach(CSymbol s, csymbolSet)
  {
      outSet.insert(s.GetString());
  }
  return outSet;
}

QSet<QString> CTerminal::CTerminalQSetToQStringQSet(const QSet<CTerminal> &csymbolSet)
{
  QSet<QString> outSet;
  foreach(CSymbol s, csymbolSet)
  {
      outSet.insert(s.GetString());
  }
  return outSet;
}

QSet<QString> CNonTerminal::CNonTerminalQSetToQStringQSet(const QSet<CNonTerminal> &csymbolSet)
{
  QSet<QString> outSet;
  foreach(CSymbol s, csymbolSet)
  {
      outSet.insert(s.GetString());
  }
  return outSet;
}
