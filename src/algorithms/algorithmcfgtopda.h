#ifndef ALGORITHMCFGTOPDA_H
#define ALGORITHMCFGTOPDA_H

#include "finite_machine/pushdownautomata.h"
#include "CFG/contextfreegrammar.h"
#include <algorithms/algorithm.h>
#include <CFG/cfgwidget.h>
#include <PDA/pdawidget.h>
#include <widgets/variableswidget.h>

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
    void SetInputCfg(CContextFreeGrammar* cfg);
    void SetInputPda(CPushDownAutomata* pda);
    CContextFreeGrammar GetCfg();
    CPushDownAutomata GetPda();

public slots:
    void SetMode(AlgorithmModes mode);
    void PrevStep();
    void NextStep();
    void CheckSolution();
    void ShowCorrectSolution();
    void ShowUserSolution();
    void ToBegin();
    void ToEnd();
    void CfgChangedSlot(const CContextFreeGrammar& cfg);
    void PdaChangedSlot(CPushDownAutomata* pda);

private:
    typedef struct step
    {
//        int num;
//        int actInstruction;
//        int prewInstruction;
//        FiniteAutomata DFA;
//        QSet <QString> act_state;
//        QSet<QString> discovered_state;
//        QSet< QSet<QString> > Q_new;
//        QString a;
//        QString p;
//        QString q;
//        ComputationalRules r;
//        ComputationalRules r_prime;
//        QStringList alphabet;
//        QSet<ComputationalRules> rules;
//        QSet<ComputationalRules> rules_prime;
    } SStep;

    QList<SStep> m_history;
    void ResetAlgorithm();
    void ComputeCorrectSolution();
    CPushDownAutomata m_pda;
    CPushDownAutomata m_userPda;
    CPushDownAutomata m_correctPda;
    CContextFreeGrammar m_cfg;
    QString m_pdaActInputAplhabetSymbol;
    CCFGRule m_actRule;
    QSet<CCFGRule>::const_iterator m_cfgRulesIter;
    QSet<CTerminal>::const_iterator m_inputAlphabetIter;
};

#endif // ALGORITHMCFGTOPDA_H
