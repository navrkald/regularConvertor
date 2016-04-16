#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"

#define START_STATE "s"

//template <typename To, typename From>
//To container_cast(From && from) {
//    using std::begin; using std::end; // Koenig lookup enabled
//    return To(begin(from), end(from));
//}

class CAlgorithmCFGtoPDA
{
public:
  CAlgorithmCFGtoPDA(const CPushDownAutomata& pda, const CContextFreeGrammar& cfg) :
      m_pda(pda), m_grammar(cfg), m_actInstruction(SET_START_STATE)/*, m_inputAlphabet(m_pda.GetSortedAlphabet())*/ {}
  CAlgorithmCFGtoPDA() : m_actInstruction(SET_START_STATE)/*, m_inputAlphabet(m_pda.GetSortedAlphabet())*/ {}

enum TInstruction{
    SET_START_STATE = 0,
    SET_INPUT_ALPHABET,
    SET_STACK_ALPHABET,
    SET_PDA_RULES_FROM_INPUT_ALPHABET,
    SET_PDA_RULES_FROM_CFG_RULES,
    SET_FINITE_STATE,
    END_INSTRUCTION,
};

public:
void ComputeNextStep()
{
    switch(m_actInstruction)
    {
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
