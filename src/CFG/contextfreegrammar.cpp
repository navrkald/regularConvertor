#include "contextfreegrammar.h"
#include <htmlcreator.h>
#include <algorithms/constants.h>
#include <QtCore>

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
    nonTerminal,
	epsilon
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
        else return E_PARSING_CFG_RULE;
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
        else return E_PARSING_CFG_RULE;
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
		else if (charter == EPSILON) {
			tmpTerminal.Append(charter);
			rule.AppendSymbolToRightSide(tmpTerminal);
			tmpTerminal.Clear();
			state = epsilon;
		}
        else return E_PARSING_CFG_RULE;
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
	  case epsilon:
		  if (charter.isSpace()) continue;
		  else if (charter == '|') {
			  rules.insert(rule);
			  rule.ClearRightSide();
			  state = rightSideRule;
		  }
		  else {
			  return E_PARSING_CFG_RULE;
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
  if(rules.isEmpty()) return E_PARSING_CFG_RULE;

  return E_NO_ERROR;
}

QString CCFGRule::ToString() const
{
    QString ruleString = QString("<%1> ::=").arg(m_leftNonTerminal.GetString());
    foreach(const CSymbol& symbol, m_rightString){
        switch (symbol.GetType()) {
            case CSymbol::TType::terminal:
				if (symbol.GetString() == EPSILON) {
					ruleString += QString(" %1 ").arg(symbol.GetString());
				}
				else {
					ruleString += QString(" \"%1\" ").arg(symbol.GetString());
				}
                break;
            case CSymbol::TType::nonTerminal:
                ruleString += QString(" <%1> ").arg(symbol.GetString());
                break;
            case CSymbol::TType::symbol:
            default:
                // This should never happend!!!
                ruleString += QString(" UNKNOWN(%1) ").arg(symbol.GetString());
                break;
        }
    }
    return ruleString;
}

QString CCFGRule::ToHtml() const {
    return CHtmlCreator::ConvertTextToHtml(ToString());
}

QDataStream & CCFGRule::WriteToQDataStream(QDataStream & out) const
{
	return out << m_leftNonTerminal << m_rightString;
}

QDataStream & CCFGRule::ReadFromQDataStream(QDataStream & in)
{
	return in >> m_leftNonTerminal >> m_rightString;
}

QDataStream& operator<<(QDataStream & out, const CCFGRule& r)
{
	return r.WriteToQDataStream(out);
}

QDataStream& operator>>(QDataStream & in, CCFGRule& r)
{
	return r.ReadFromQDataStream(in);
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

QString CContextFreeGrammar::TerminalAlphabetToString() const
{
  QString result;
  foreach(CSymbol s, m_terminalsAlphabet)
  {
    result.append(s.GetString());
    result += ", ";
  }
  return result.left(result.length() - 2);
}

QString CContextFreeGrammar::NonTerminalAlphabetToString() const
{
  QString result;
  foreach(CSymbol s, m_nonTerminalsAlphabet)
  {
    result.append(s.GetString());
    result += ", ";
  }
  return result.left(result.length() - 2);
}

QSet<QString> CContextFreeGrammar::GetBothTerminalAndNonterminalAlphabet()
{
    QSet<QString> terninals = CTerminal::CTerminalQSetToQStringQSet(GetTerminalAlphabet());
    QSet<QString> nonTerminals = CNonTerminal::CNonTerminalQSetToQStringQSet(GetNonTerminalAlphabet());
    QSet<QString> outSet = terninals + nonTerminals;
    return outSet;
}

int CContextFreeGrammar::GetRulesCount()
{
  return m_rules.size();
}

ErrorCode CContextFreeGrammar::GetFromBackusNaurForm(QString sContextFreeGrammar)
{
  Clear();
  ErrorCode err = E_NO_ERROR;
  QStringList lines = sContextFreeGrammar.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
  bool isLeftNonTerminalEmpty = true;
  foreach(QString line, lines)
  {
    line = line.trimmed();
    QSet<CCFGRule> rules;
    if(!line.isEmpty())
    {
      if(E_NO_ERROR == (err = CCFGRule::GetRulesFromString(rules, line)))
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
            if ( s.GetType() == CSymbol::terminal && s.GetString() != EPSILON ){
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
      else{
        return err;
      }
    }
  }
  return E_NO_ERROR;
}

QList<CCFGRule> CContextFreeGrammar::GetListOfRules() const{
    QList<CCFGRule> listOfRules = m_rules.toList();
    // We need rule which starts with has left nonterminal equal m_startNonTerminal
    if(!m_startNonTerminal.IsEmpty()){
        for(int i = 0; i < listOfRules.count(); i++){
            if(listOfRules.at(i).m_leftNonTerminal == m_startNonTerminal){
                listOfRules.move(i, 0);
                break;
            }
        }
    }
    return listOfRules;
}

QString CContextFreeGrammar::ToBackusNaurForm() const
{
    QString backusNaurString;
    QList<CCFGRule> sortedRules = GetListOfRules();
    foreach(const CCFGRule& rule, sortedRules){
        backusNaurString += rule.ToString() + "\n";
    }
    return backusNaurString;
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

void CContextFreeGrammar::Clear() {
	m_nonTerminalsAlphabet.clear();
	m_terminalsAlphabet.clear();
	m_startNonTerminal.Clear();
	m_rules.clear();
}

QDataStream& CContextFreeGrammar::WriteToQDataStream(QDataStream& stream) const {
	return stream << m_nonTerminalsAlphabet << m_terminalsAlphabet << m_startNonTerminal << m_rules;
}

QDataStream& CContextFreeGrammar::ReadFromQDataStream(QDataStream& stream){
	return stream >> m_nonTerminalsAlphabet >> m_terminalsAlphabet >> m_startNonTerminal >> m_rules;
}

QDataStream & operator<<(QDataStream & out, const CContextFreeGrammar& g)
{
	return g.WriteToQDataStream(out);
}

QDataStream & operator>>(QDataStream & in, CContextFreeGrammar& g)
{
	return g.ReadFromQDataStream(in);
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

QDataStream& CSymbol::WriteToQDataStream(QDataStream & out) const
{
	return out << (quint32) m_type << m_symbol;
}

QDataStream& CSymbol::ReadFromQDataStream(QDataStream & in)
{
	quint32 tmpType;
	in >> tmpType >> m_symbol;
	m_type = (TType)tmpType;
	return in;
}

QDataStream & operator>>(QDataStream & in, CTerminal & s)
{
	return s.ReadFromQDataStream(in);
}

QDataStream & operator<<(QDataStream & out, const CTerminal & s)
{
	return s.WriteToQDataStream(out);
}

QDataStream & operator>>(QDataStream & in, CNonTerminal & s)
{
	return s.ReadFromQDataStream(in);
}

QDataStream & operator<<(QDataStream & out, const CNonTerminal & s)
{
	return s.WriteToQDataStream(out);
}

QDataStream & operator>>(QDataStream & out, CSymbol& s)
{
	return s.ReadFromQDataStream(out);
}

QDataStream & operator<<(QDataStream & out, const CSymbol & s)
{
	return s.WriteToQDataStream(out);
}
