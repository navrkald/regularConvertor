#include "contextfreegrammar.h"

ErrorCode CCFGRule::GetRulesFromString(QSet<CCFGRule>& rules, QString sRule)
{
  typedef enum state{
    start,
    leftStartNonTerminalTermminal,
    leftEndNonTerminal,
    rightSideRuleDelimiter1,
    rightSideRuleDelimiter2,
    rightSideRule,
    terminal,
    nonTerminal
  }TState;
  TState state = start;
  CCFGRule rule;
  CNonTerminal leftNonTerminal;
  CNonTerminal tmpNonTerminal;
  CTerminal tmpTerminal;
  foreach(QChar charter, sRule)
  {
    switch(state)
    {
      case start:
        if(charter == '<') state = leftStartNonTerminalTermminal;
        break;
      case leftStartNonTerminalTermminal:
        if(charter != '>')
          leftNonTerminal.Append(charter);
        else{
            state = leftEndNonTerminal;
            leftNonTerminal.Trim();
            if(leftNonTerminal.IsEmpty()) return E_PARSING_CFG_RULE;
            else rule.SetLeftNonTerminal(leftNonTerminal);
        }
        break;
      case leftEndNonTerminal:
        if(charter.isSpace()) continue;
        else if(charter == ':') state = rightSideRuleDelimiter1;
        break;
      case rightSideRuleDelimiter1:
        if(charter == ':') state = rightSideRuleDelimiter2;
        else return E_PARSING_CFG_RULE;
        break;
      case rightSideRuleDelimiter2:
        if(charter == '=') state = rightSideRule;
        else return E_PARSING_CFG_RULE;
        break;
      case rightSideRule:
        tmpTerminal.Clear();
        tmpNonTerminal.Clear();
        if(charter == '<') state = nonTerminal;
        else if(charter == '"') state = terminal;
        else if(charter == '|')
        {
          if(!rule.IsRightSideEmpty())
          {
            rules.insert(rule);
            rule.ClearRightSide();
          }
        }
        else if(charter.isSpace()) continue;
        break;
      case terminal:
        if(charter != '"')
          tmpTerminal.Append(charter);
        else
        {
          state = rightSideRule;
          rule.AppendSymbolToRightSide(tmpTerminal);
          tmpTerminal.Clear();
        }
        break;
      case nonTerminal:
        if(charter != '>')
          tmpNonTerminal.Append(charter);
        else
        {
          state = rightSideRule;
          tmpNonTerminal.Trim();
          if(!tmpNonTerminal.IsEmpty())
          {
            rule.AppendSymbolToRightSide(tmpNonTerminal);
            tmpNonTerminal.Clear();
          }
          else return E_PARSING_CFG_RULE;
        }
        break;
      }
  }
  // add last rule on row
  if(!rule.IsRightSideEmpty())
  {
    rules.insert(rule);
    rule.ClearRightSide();
  }
  return E_NO_ERROR;
}

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

ErrorCode CContextFreeGrammar::GetFromString(QString sContextFreeGrammar)
{
  QStringList lines = sContextFreeGrammar.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
  bool isLeftNonTerminalEmpty = true;
  foreach(QString line, lines)
  {
    line = line.trimmed();
    QSet<CCFGRule> rules;
    if(!line.isEmpty())
    {
      if(E_NO_ERROR == CCFGRule::GetRulesFromString(rules, line))
      {
        foreach(CCFGRule rule, rules)
        {
          if(isLeftNonTerminalEmpty){
            m_startNonTerminal = rule.m_leftNonTerminal;
            isLeftNonTerminalEmpty = false;
          }
          m_nonTerminalsAlphabet.insert(rule.m_leftNonTerminal);
          foreach(CSymbol s, rule.m_rightString)
          {
            if ( s.GetType() == CSymbol::terminal ){
              m_terminalsAlphabet.insert((CTerminal)s);
            }
            else if ( s.GetType() == CSymbol::nonTerminal ) {
              m_nonTerminalsAlphabet.insert((CNonTerminal)s);
            }

              Q_ASSERT_X(s.GetType() != CSymbol::symbol, "CContextFreeGrammar::GetFromString", "Symbol can' be type CSymbol!");
          }
          m_rules.insert(rule);
        }
      }
    }
  }
  return E_NO_ERROR;
}

//bool operator==(const CContextFreeGrammar &g1, const CContextFreeGrammar &g2)
//{
//  return g1.m_nonTerminalsAlphabet == g2.m_nonTerminalsAlphabet &&
//         g1.m_rules == g2.m_rules &&
//         g1.m_startNonTerminal == g2.m_startNonTerminal &&
//         g1.m_terminalsAlphabet == g2.m_terminalsAlphabet;
//}

bool CContextFreeGrammar::operator==(const CContextFreeGrammar &g) const
{
  return this->m_nonTerminalsAlphabet == g.m_nonTerminalsAlphabet &&
         this->m_rules == g.m_rules &&
         this->m_startNonTerminal == g.m_startNonTerminal &&
         this->m_terminalsAlphabet == g.m_terminalsAlphabet;
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
