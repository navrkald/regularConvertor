#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"

#define START_STATE "s"

class CAlgorithmCFGtoPDA
{
public:
    CAlgorithmCFGtoPDA() : m_actInstruction(SET_START_STATE), m_inputAlphabet(m_pda.GetSortedAlphabet()) {}

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
            m_pda.SetStartState(START_STATE);
            m_actInstruction = SET_INPUT_ALPHABET;
            break;
        }
        case SET_INPUT_ALPHABET:
        {
            m_pda.SetAplhabet(m_grammar.GetTerminalAlphabet());
            m_actInstruction = SET_STACK_ALPHABET;
            break;
        }
        case SET_STACK_ALPHABET:
        {
            m_pda.SetStackAlphabet(m_grammar.GetBothTerminalAndNonterminalAlphabet());
            m_actInstruction = SET_PDA_RULES_FROM_CFG_RULES;
            m_inputAlphabetIndex = 0;
            break;
        }
        case SET_PDA_RULES_FROM_INPUT_ALPHABET:
        {
            if(m_inputAlphabet.size() >= m_inputAlphabetIndex)
            {
                m_actInstruction = SET_PDA_RULES_FROM_CFG_RULES;
                m_cfgRulesIter = m_grammar.m_rules.constBegin();
            }
            else
            {
                m_pdaActInputAplhabetSymbol = m_inputAlphabet[m_inputAlphabetIndex];
                m_pda.AddPDARule(m_pda.GetStartState(), m_pda.GetStartState(), EPSILON, m_pdaActInputAplhabetSymbol, m_pdaActInputAplhabetSymbol);
                m_inputAlphabetIndex++;
            }
            break;
        }
        case SET_PDA_RULES_FROM_CFG_RULES:
        {
            if(m_cfgRulesIter != m_grammar.m_rules.constEnd())
            {
                m_actRule = *m_cfgRulesIter;
                m_pda.AddPDARule(START_STATE, START_STATE, EPSILON, m_actRule.leftTerminal.m_symbol, m_actRule.GetRevertedRightRule());
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


private:
    TInstruction m_actInstruction;
    QVector<QString> m_inputAlphabet;
    QString m_pdaActInputAplhabetSymbol;
    int m_inputAlphabetIndex;
    CCFGRule m_actRule;
    QSet<CCFGRule>::const_iterator m_cfgRulesIter;
    CPushDownAutomata m_pda;
    CContextFreeGrammar m_grammar;
};

#endif // ALGORITHMCFGTOPDA_H
