#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"
#include <algorithms/algorithm.h>
#include <CFG/cfgwidget.h>
#include <PDA/pdawidget.h>
#include <widgets/variableswidget.h>

#define START_STATE "s"

class CAlgorithmCFGtoPDA : public CAlgorithm
{
    Q_OBJECT
public:
    enum TInstruction{
        HEADER = 0,
        SET_START_STATE,
        SET_INPUT_ALPHABET,
        SET_STACK_ALPHABET,
        SET_PDA_RULES_HEADER,
        FOREACH_PDA_RULES_FROM_INPUT_ALPHABET,
        PDA_RULE_FROM_INPUT_ALPHABET,
        FOREACH_PDA_RULES_FROM_CFG_RULES,
        SET_PDA_RULE_FROM_CFG_RULE,
        SET_FINITE_STATE,
        END_INSTRUCTION,
    };


CAlgorithmCFGtoPDA() {
    m_actInstruction = HEADER;
    m_prevInstruction = HEADER;
}
  CAlgorithmCFGtoPDA(const CPushDownAutomata& pda, const CContextFreeGrammar& cfg) :
      m_pda(pda), m_cfg(cfg)
{
  m_actInstruction = HEADER;
  m_prevInstruction = HEADER;
}

void Init(CAlgorithmWidget* algorithmWidget, CCfgWidget* cfgWidget, CPdaWidget* pdaWidget, CVariablesWidget* variablesWidget);

virtual void InitInstructions();
virtual void RemoveFuture(){;}

protected:
    CAlgorithmWidget* m_algorithmWidget;
    CCfgWidget* m_cfgWidget;
    CPdaWidget* m_pdaWidget;
    CVariablesWidget* m_variablesWidget;

    void ShowVariables();
    void SaveStep();
public:
    void ComputeNextStep();

    QString GetDebugVariablesInHtml(TInstruction instruction);

    TInstruction GetActInstruction() {return (TInstruction) m_actInstruction;}
    CContextFreeGrammar GetCfg();
    CPushDownAutomata GetPda();

public slots:
    void SetMode(AlgorithmModes _mode);
    void PrevStep();
    void NextStep();
    void CheckSolution();
    void ShowCorrectSolution();
    void ShowUserSolution();
    void ToBegin();
    void ToEnd();
    void SetCfg(CContextFreeGrammar* cfg);
    void SetPda(CPushDownAutomata* pda);   

private:
    CPushDownAutomata m_pda;
    CContextFreeGrammar m_cfg;
//    TInstruction m_actInstruction;
//    TInstruction m_prevInstruction;
    QString m_pdaActInputAplhabetSymbol;
    CCFGRule m_actRule;
    QSet<CCFGRule>::const_iterator m_cfgRulesIter;
    QSet<QString>::const_iterator m_inputAlphabetIter;
};

#endif // ALGORITHMCFGTOPDA_H
