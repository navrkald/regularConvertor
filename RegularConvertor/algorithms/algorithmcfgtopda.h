#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"


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

TInstruction m_actInstruction;

void ComputeNextStep()
{
    switch(m_actInstruction)
    {
        case SET_START_STATE:
        {
            m_pda.SetStartState("s");
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
                m_cfgRuleIndex = 0;
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
        case SET_START_STATE:
            return m_pda.PrintHtmlStates();
            break;
        case SET_INPUT_ALPHABET:

            break;
    }
}



private:
    QVector<QString> m_inputAlphabet;
    QString m_pdaActInputAplhabetSymbol;
    int m_inputAlphabetIndex;
    int\m_cfgRuleIndex;
    CPushDownAutomata m_pda;
    CContextFreeGrammar m_grammar;
};

#endif // ALGORITHMCFGTOPDA_H
