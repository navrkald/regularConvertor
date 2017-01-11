#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"
#include <algorithms/algorithm.h>

#define START_STATE "s"

//template <typename To, typename From>
//To container_cast(From && from) {
//    using std::begin; using std::end; // Koenig lookup enabled
//    return To(begin(from), end(from));
//}

class CAlgorithmCFGtoPDA
{
public:
  CAlgorithmCFGtoPDA() {m_actInstruction = HEADER;}
  CAlgorithmCFGtoPDA(const CPushDownAutomata& pda, const CContextFreeGrammar& cfg) :
      m_pda(pda), m_grammar(cfg), m_actInstruction(SET_START_STATE)/*, m_inputAlphabet(m_pda.GetSortedAlphabet())*/
{
  m_actInstruction = HEADER;
  //prewInstruction = HEADER;
  //instruction_count = END_INSTRUCTION+1;
  //initInstructions();
  //initBreakpoints(instruction_count);
}
//  CAlgorithmCFGtoPDA() : m_actInstruction(SET_START_STATE)/*, m_inputAlphabet(m_pda.GetSortedAlphabet())*/ {}

enum TInstruction{
    HEADER = 0,
    SET_START_STATE,
    SET_INPUT_ALPHABET,
    SET_STACK_ALPHABET,
    SET_PDA_RULES_FROM_INPUT_ALPHABET,
    SET_PDA_RULES_FROM_CFG_RULES,
    SET_FINITE_STATE,
    END_INSTRUCTION,
};

virtual void initInstructions()
{
//  instructions.resize(instruction_count);
//  instructions[HEADER] =                  tr("<b>Input:</b> FA without ε-rules <i>M</i>=(Q, Σ, R, s, F)<br><b>Output:</b> DKA M'=(Q<sub>d</sub>, Σ, R<sub>d</sub>, s<sub>d</sub>, F<sub>d</sub>)");
//  instructions[SET_START_STATE] =                tr("s<sub>d</sub> = {s}; Q<sub>new</sub> = {s<sub>d</sub>}; R<sub>d</sub> = ∅; Q<sub>d</sub> = Q<sub>new</sub>; F<sub>d</sub> = ∅");
//  instructions[SET_INPUT_ALPHABET] =                      tr("<b>do</b>");
//  instructions[SET_STACK_ALPHABET] =                 INDENT     + tr("Q' ∈ Q<sub>new</sub>; Q<sub>new</sub> = Q<sub>new</sub> - {Q'};");
//  instructions[SET_PDA_RULES_FROM_INPUT_ALPHABET] =               INDENT     + tr("<b>for each</b> a ∈ Σ <b>do</b>");
//  instructions[SET_PDA_RULES_FROM_CFG_RULES]=      INDENT  INDENT      + tr("Q'' = ∅");
//  instructions[SET_FINITE_STATE] =       INDENT  INDENT     + tr("<b>for each</b> r = {p a → q} <b>where</b> p ∈ Q'");
//  instructions[END_INSTRUCTION] =          INDENT  INDENT  INDENT     + tr("Q'' ∈ Q'' ∪ {q}");
//  instructions[IF_Q_NEW] =                INDENT  INDENT     + tr("<b>if</b> Q'' ∉ Q<sub>d</sub> ∪ {∅} <b>then</b> Q<sub>new</sub> = Q<sub>new</sub> ∪ {Q''}; Q<sub>d</sub> = Q<sub>d</sub> ∪ {Q''};");
//  instructions[IF_DOUBLE_PRIME_Q] =       INDENT  INDENT     + tr("<b>if</b> Q'' ≠ ∅ <b>then</b> R<sub>d</sub> = R<sub>d</sub> ∪ r'={Q' a → Q''};");
//  instructions[IF_FINAL] =                INDENT      + tr("<b>if</b> Q' ∩ F ≠ ∅ <b>then</b> F<sub>d</sub> = F<sub>d</sub> ∪ {Q'}");
//  instructions[WHILE_NEW] =               tr("<b>until</b> Q<sub>new</sub> = ∅");
}
virtual void removeFuture(){;}


public:
void ComputeNextStep()
{
    switch(m_actInstruction)
    {
        case HEADER:
          break;
        case SET_START_STATE:
        {
            m_pda.addState(START_STATE);
            m_pda.SetStartState(START_STATE);
            m_actInstruction = SET_INPUT_ALPHABET;
            break;
        }
        case SET_INPUT_ALPHABET:
        {
            m_pda.SetAplhabet(CTerminal::CTerminalQSetToQStringQSet((m_grammar.GetTerminalAlphabet())));
            m_actInstruction = SET_STACK_ALPHABET;
            break;
        }
        case SET_STACK_ALPHABET:
        {
            m_pda.SetStackAlphabet(m_grammar.GetBothTerminalAndNonterminalAlphabet());
            m_actInstruction = SET_PDA_RULES_FROM_INPUT_ALPHABET;
            m_inputAlphabetIter = m_pda.GetAlphabet().constBegin();
            break;
        }
        case SET_PDA_RULES_FROM_INPUT_ALPHABET:
        {
            if(m_inputAlphabetIter != m_pda.GetAlphabet().constEnd())
            {
                m_pdaActInputAplhabetSymbol = *m_inputAlphabetIter;
                m_pda.AddPDARule(m_pda.GetStartState(), m_pda.GetStartState(), m_pdaActInputAplhabetSymbol, m_pdaActInputAplhabetSymbol, EPSILON);
                m_inputAlphabetIter++;
            }
            else
            {
                m_actInstruction = SET_PDA_RULES_FROM_CFG_RULES;
                m_cfgRulesIter = m_grammar.GetRules().constBegin();
            }
            break;
        }
        case SET_PDA_RULES_FROM_CFG_RULES:
        {
            if(m_cfgRulesIter != m_grammar.GetRules().constEnd())
            {
                m_actRule = *m_cfgRulesIter;
                m_pda.AddPDARule(START_STATE, START_STATE, EPSILON, m_actRule.m_leftNonTerminal, m_actRule.GetRevertedRightRule());
                m_cfgRulesIter++;
            }
            else
            {
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        }
        case SET_FINITE_STATE:
        {
            m_actInstruction = END_INSTRUCTION;
            break;
        }
        case END_INSTRUCTION:
        {
            break;
        }
    }
}

QString GetDebugVariablesInHtml(TInstruction instruction)
{
    switch(instruction)
      {
      case CAlgorithmCFGtoPDA::HEADER:
        break;
      case CAlgorithmCFGtoPDA::SET_STACK_ALPHABET:
        break;
      case CAlgorithmCFGtoPDA::SET_PDA_RULES_FROM_INPUT_ALPHABET:
        break;
      case CAlgorithmCFGtoPDA::SET_PDA_RULES_FROM_CFG_RULES:
        break;
      case CAlgorithmCFGtoPDA::SET_FINITE_STATE:
        break;
      case CAlgorithmCFGtoPDA::END_INSTRUCTION:
        break;
      case SET_START_STATE:
        return m_pda.PrintHtmlStates();
        break;
      case SET_INPUT_ALPHABET:
        break;
      }
}

TInstruction GetActInstruction() {return m_actInstruction;}

private:
    CPushDownAutomata m_pda;
    CContextFreeGrammar m_grammar;
    TInstruction m_actInstruction;
    //QVector<QString> m_inputAlphabet;
    QString m_pdaActInputAplhabetSymbol;
    //int m_inputAlphabetIndex;
    CCFGRule m_actRule;
    QSet<CCFGRule>::const_iterator m_cfgRulesIter;
    QSet<QString>::const_iterator m_inputAlphabetIter;
};

#endif // ALGORITHMCFGTOPDA_H
