#include "CFG/cfg_test.h"

CfgTest::CfgTest(QObject *parent) : QObject(parent)
{

}

void CfgTest::initTestCase()
{
  m_backusNaurFrom1 =
      "<S> ::= <A> \"a\" | <B> \"c\" | <D> \r\n"
      "<Q> ::= <A> \"a\" \r\n"
      "<L> ::= <B> \"c\" | <D> \r\n";
  QSet<CNonTerminal> nonTerminalAlphabet(
  {
    CNonTerminal("S"),
    CNonTerminal("Q"),
    CNonTerminal("L"),
    CNonTerminal("A"),
    CNonTerminal("B"),
    CNonTerminal("D")
  });
  QSet<CTerminal> terminalAlphabet(
  {
    CTerminal("a"),
    CTerminal("c"),
  });
  QSet<CCFGRule> rules({
    CCFGRule(CNonTerminal("S"), QVector<CSymbol>({CNonTerminal("A"), CTerminal("a")})),
    CCFGRule(CNonTerminal("S"), QVector<CSymbol>({CNonTerminal("B"), CTerminal("c")})),
    CCFGRule(CNonTerminal("S"), QVector<CSymbol>({CNonTerminal("D")})),
    CCFGRule(CNonTerminal("Q"), QVector<CSymbol>({CNonTerminal("A"), CTerminal("a")})),
    CCFGRule(CNonTerminal("L"), QVector<CSymbol>({CNonTerminal("B"), CTerminal("c")})),
    CCFGRule(CNonTerminal("L"), QVector<CSymbol>({CNonTerminal("D")}))
  });

  m_grammar1Check.SetNonterminalsAlphabet(nonTerminalAlphabet);
  m_grammar1Check.SetTerminalsAlphabet(terminalAlphabet);
  m_grammar1Check.SetStartNonTerminal(CNonTerminal("S"));
  m_grammar1Check.SetRules(rules);
}

void CfgTest::CfgFromBackusNaurForm()
{
  ErrorCode err = m_grammar1.GetFromString(m_backusNaurFrom1);
  QCOMPARE(E_NO_ERROR, err);
  QCOMPARE(m_grammar1Check, m_grammar1);
}

