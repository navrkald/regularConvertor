#include "algorithmcfgtopda.h"

#include <QVector>
#include <algorithms/htmldelegate.h>

void CAlgorithmCFGtoPDA::Init(CAlgorithmWidget* algorithmWidget, CCfgWidget* cfgWidget, CPdaWidget* pdaWidget, CVariablesWidget* variablesWidget)
{
    m_algorithmWidget = algorithmWidget;
    m_cfgWidget = cfgWidget;
    m_pdaWidget = pdaWidget;
    m_variablesWidget = variablesWidget;
    m_actInstruction = HEADER;

    SetupAlgorithmWidget(m_algorithmWidget);

    m_actInstruction = HEADER;
    m_prevInstruction = HEADER;
    InitInstructions();
    InitBreakpoints(END_INSTRUCTION);
    setColumnCount(1);
    setRowCount(END_INSTRUCTION);
    m_variablesWidget->SetText("");

    for(int i = 0; i < m_instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,m_instructions[i],Qt::EditRole);
        setData(index,true,CAlgorithm::hasBreakpointRole);
        setData(index,false,CAlgorithm::breakpointRole);
        switch(i)
        {
            case HEADER:
            case SET_PDA_RULES_HEADER:
                setData(index,false,CAlgorithm::hasBreakpointRole);
                break;
        }
    }

    //
    // Connect algorithm buttons.
    //
    connect(this->m_algorithmWidget, SIGNAL(playPressed(int)),this,SLOT(RunAlgorithm(int)));
    connect(this->m_algorithmWidget, SIGNAL(stopPressed()),this,SLOT(Stop()));
    connect(this->m_algorithmWidget, SIGNAL(prewPressed()),this,SLOT(PrevStep()));
    connect(this->m_algorithmWidget, SIGNAL(nextPressed()),this,SLOT(NextStep()));
    connect(this->m_algorithmWidget, SIGNAL(checkSolutionPressed()), this, SLOT(CheckSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(ShowCorrectSolution()));
    connect(this->m_algorithmWidget, SIGNAL(showUserSolutionPressed()), this, SLOT(ShowUserSolution()));
    connect(this->m_algorithmWidget, SIGNAL(beginPressed()), this, SLOT(ToBegin()));
    connect(this->m_algorithmWidget, SIGNAL(endPressed()), this, SLOT(ToEnd()));

    //
    // Connect timers.
    //
    connect(m_playTimer, SIGNAL(timeout()), this, SLOT(NextStep()));
    connect(m_CheckStepTimer, SIGNAL(timeout()), this, SLOT(CheckSolution()));

    // Connect CFG and PDA widgets
    connect(m_cfgWidget, SIGNAL(CfgChanged(CContextFreeGrammar*)), this, SLOT(SetCfg(CContextFreeGrammar*)));
    connect(m_pdaWidget, SIGNAL(PdaChanged(CPushDownAutomata*)), this, SLOT(SetPda(CPushDownAutomata*)));

    m_pdaWidget->SetPda(new CPushDownAutomata());

    m_algorithmWidget->enableShowButton();
}

void CAlgorithmCFGtoPDA::InitInstructions()
{
    m_instructions.resize(END_INSTRUCTION);
    m_instructions[HEADER] =                                tr("<b>Input:</b> CFG <i>G</i>=(N, T, P, S)<br><b>Output:</b> PDA M'=(Q, Σ, Γ, R, s, S, F)");
    m_instructions[SET_START_STATE] =                       "Q = {s}";
    m_instructions[SET_INPUT_ALPHABET] =                    "Σ = T";
    m_instructions[SET_STACK_ALPHABET] =                    "Γ = N ∪ T";
    m_instructions[SET_PDA_RULES_HEADER] =                  tr("Construction of set R");
    m_instructions[FOREACH_PDA_RULES_FROM_INPUT_ALPHABET] = INDENT + tr("<b>for each</b> a ∈ Σ");
    m_instructions[PDA_RULE_FROM_INPUT_ALPHABET] =          INDENT INDENT + tr("add asa → to R");
    m_instructions[FOREACH_PDA_RULES_FROM_CFG_RULES] =      INDENT + tr("<b>for each</b> A → x ∈ P");
    m_instructions[SET_PDA_RULE_FROM_CFG_RULE] =            INDENT INDENT + tr("add As → ys to R where y = reversal(x)");
    m_instructions[SET_FINITE_STATE] =                      "F = ∅";
}

void CAlgorithmCFGtoPDA::ComputeNextStep()
{
    // TODO: Check if CFG is valid

    m_algorithmWidget->enablePrev();
    switch(m_prevInstruction){
        case HEADER:
            m_actInstruction = SET_START_STATE;
            break;
        case SET_START_STATE:
            m_actInstruction = SET_INPUT_ALPHABET;
            break;
        case SET_INPUT_ALPHABET:
            m_actInstruction = SET_STACK_ALPHABET;
            break;
        case SET_STACK_ALPHABET:
            if(!m_cfg.GetTerminalAlphabet().empty()){
                m_actInstruction = FOREACH_PDA_RULES_FROM_INPUT_ALPHABET;
            }
            else if(m_cfg.GetRulesCount() > 0){
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
            }
            else{
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        case FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
            if(m_inputAlphabetIter != m_cfg.GetTerminalAlphabet().end()){
                m_actInstruction = PDA_RULE_FROM_INPUT_ALPHABET;
            }
            else if(m_cfg.GetRulesCount() > 0){
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
            }
            else{
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        case PDA_RULE_FROM_INPUT_ALPHABET:
            break;
        case FOREACH_PDA_RULES_FROM_CFG_RULES:
            break;
        case SET_PDA_RULE_FROM_CFG_RULE:
            break;
        case SET_FINITE_STATE:
            break;
        case END_INSTRUCTION:
            break;
        default:
            break;
    }

	switch (m_prevInstruction)
	{
	default:
		break;
	}

    switch(m_actInstruction)
    {
        case HEADER:
            m_actInstruction = SET_START_STATE;
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
            m_pda.SetAplhabet(CTerminal::CTerminalQSetToQStringQSet((m_cfg.GetTerminalAlphabet())));
            m_actInstruction = SET_STACK_ALPHABET;
            break;
        }
        case SET_STACK_ALPHABET:
        {
            m_pda.SetStackAlphabet(m_cfg.GetBothTerminalAndNonterminalAlphabet());
            m_actInstruction = FOREACH_PDA_RULES_FROM_INPUT_ALPHABET;
            break;
        }
        case FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
        {
            if(m_prevInstruction == SET_STACK_ALPHABET){
                m_inputAlphabetIter = m_pda.GetAlphabet().constBegin();
            } else{
                m_inputAlphabetIter++;
            }
            m_pdaActInputAplhabetSymbol = *m_inputAlphabetIter;
            break;
        }
        case PDA_RULE_FROM_INPUT_ALPHABET:
        {
            if(m_inputAlphabetIter != m_pda.GetAlphabet().constEnd())
            {
                m_pda.AddPDARule(m_pda.GetStartState(), m_pda.GetStartState(), m_pdaActInputAplhabetSymbol, m_pdaActInputAplhabetSymbol, EPSILON);
                m_actInstruction = FOREACH_PDA_RULES_FROM_INPUT_ALPHABET;
            }
            else
            {
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
                m_cfgRulesIter = m_cfg.GetRules().constBegin();
            }
            break;
        }
        case FOREACH_PDA_RULES_FROM_CFG_RULES:
        {
            if(m_prevInstruction == PDA_RULE_FROM_INPUT_ALPHABET || m_prevInstruction == SET_STACK_ALPHABET)
            {
                m_cfgRulesIter = m_cfg.GetRules().constBegin();
            }
            else
            {
                m_cfgRulesIter++;
            }
            m_actRule = *m_cfgRulesIter;
            break;
        }
        case SET_PDA_RULE_FROM_CFG_RULE:
        {
            if(m_cfgRulesIter != m_cfg.GetRules().constEnd())
            {
                m_pda.AddPDARule(START_STATE, START_STATE, EPSILON, m_actRule.m_leftNonTerminal, m_actRule.GetRevertedRightRule());
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
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

    RemoveFuture();
    if(m_actInstruction != lastInstruction)
    {
        m_prevInstruction = m_actInstruction;
        SaveStep();
        if(m_breakpoints[m_actInstruction])
            m_playTimer->stop();
    }
    else
    {
        m_algorithmWidget->disableNext();
        m_playTimer->stop();
        m_pdaWidget->setCorrectStatus();
    }
    ShowVariables();
    SetActInstruction();

}

void CAlgorithmCFGtoPDA::ShowVariables(){
    m_variablesWidget->SetText(GetDebugVariablesInHtml((TInstruction)m_actInstruction));
}

QString CAlgorithmCFGtoPDA::GetDebugVariablesInHtml(CAlgorithmCFGtoPDA::TInstruction instruction)
{
    switch(instruction)
    {
        case CAlgorithmCFGtoPDA::HEADER:
            break;
        case CAlgorithmCFGtoPDA::SET_STACK_ALPHABET:
            break;
        case CAlgorithmCFGtoPDA::FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
            break;
        case CAlgorithmCFGtoPDA::PDA_RULE_FROM_INPUT_ALPHABET:
            break;
        case CAlgorithmCFGtoPDA::FOREACH_PDA_RULES_FROM_CFG_RULES:
            break;
        case CAlgorithmCFGtoPDA::SET_PDA_RULE_FROM_CFG_RULE:
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

CContextFreeGrammar CAlgorithmCFGtoPDA::GetCfg()
{
    return m_cfgWidget->GetCfg();
}

CPushDownAutomata CAlgorithmCFGtoPDA::GetPda()
{
    return m_pdaWidget->GetPda();
}

void CAlgorithmCFGtoPDA::SetMode(AlgorithmModes _mode)
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::PrevStep()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::NextStep()
{
    ComputeNextStep();
    m_pdaWidget->SetPda(&m_pda);
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::CheckSolution()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::ShowCorrectSolution()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::ShowUserSolution()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::ToBegin()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::ToEnd()
{
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::SetCfg(CContextFreeGrammar *cfg)
{
    m_cfgWidget->SetCfg(*cfg);
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::SetPda(CPushDownAutomata *pda)
{
    m_pdaWidget->SetPda(pda);
    // TODO: Implement
}

void CAlgorithmCFGtoPDA::SaveStep(){
    // TODO: Implement
}
