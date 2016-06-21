#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "reg_exp/regexp.h"
#include "reg_exp/regexpparser.h"
#include "algorithms/regexptofa.h"
#include "finite_machine/arrow.h"
#include "finite_machine/finiteautomata.h"

// To test CFG to PDA
#include "algorithms/algorithmcfgtopda.h"

void TestCfgToPda()
{
  // Initialize context free grammer
  CContextFreeGrammar g;
  CNonTerminal S("S");
  g.SetStartNonTerminal(S);
  QSet<CNonTerminal> nonterminals;
  nonterminals.insert(S);
  g.SetNonterminalsAlphabet(nonterminals);
  QSet<CTerminal> terminals;
  CTerminal leftBracket("{");
  CTerminal rightBracket("}");
  terminals.insert(leftBracket);
  terminals.insert(rightBracket);
  g.SetTerminalsAlphabet(terminals);
  QSet<CCFGRule> rules;
  QVector<CSymbol> rightRulePart1= {leftBracket, S, rightBracket};
  QVector<CSymbol> rightRulePart2 = {leftBracket, rightBracket};
  rules.insert(CCFGRule(S, rightRulePart1));
  rules.insert(CCFGRule(S, rightRulePart2));
  g.SetRules(rules);
  CPushDownAutomata pda;

  CAlgorithmCFGtoPDA algorithm(pda,g);
  do{
    algorithm.ComputeNextStep();
  }while(algorithm.GetActInstruction() != CAlgorithmCFGtoPDA::END_INSTRUCTION);
  algorithm.ComputeNextStep();
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainWindow w;
  w.show();


  return a.exec();
}



