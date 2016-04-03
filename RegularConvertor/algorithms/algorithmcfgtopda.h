#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"


class CAlgorithmCFGtoPDA
{
public:
    CAlgorithmCFGtoPDA() : m_actInstruction(SET_START_STATE), m_inputAlphabet(m_pda.GetSortedAlphabet()) {}

enum instruction{
    SET_START_STATE = 0,
    SET_INPUT_ALPHABET,
    SET_STACK_ALPHABET,
    SET_PDA_RULES_FROM_INPUT_ALPHABET,
    SET_PDA_RULES_FROM_CFG_RULES,
    SET_FINITE_STATE,
    END_INSTRUCTION,
};

instruction m_actInstruction;

void ComputeNextStep()
{
    switch(m_actInstruction)
    {
        case SET_START_STATE:
        {
            pda.SetStartState("s");
            m_actInstruction = SET_INPUT_ALPHABET;
            break;
        }
        case SET_INPUT_ALPHABET:
        {
            pda.SetAplhabet(grammar.GetTerminalAlphabet());
            m_actInstruction = SET_STACK_ALPHABET;
            break;
        }
        case SET_STACK_ALPHABET:
        {
            pda.SetStackAlphabet(GetBothTerminalAndNonterminalAlphabet());
            m_actInstruction = SET_PDA_RULES_FROM_CFG_RULES;
            m_inputAlphabetIndex = 0;
            break;
        }
        case SET_PDA_RULES_FROM_INPUT_ALPHABET:
        {
            if(m_inputAlphabet.size() >= m_inputAlphabetIndex)
            {
                m_actInstruction = SET_PDA_RULES_FROM_CFG_RULES;
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
    }
}

QString GetDebugVariablesInHtml(instruction instruction)
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
    CPushDownAutomata m_pda;
    CContextFreeGrammar m_grammar;
};

#endif // ALGORITHMCFGTOPDA_H
