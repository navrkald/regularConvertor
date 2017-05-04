#include "algorithmcfgtopda.h"

#include <QVector>
#include <algorithms/htmldelegate.h>
#include <algorithms/constants.h>
#include <htmlcreator.h>

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
	m_instructionCount = m_instructions.count();

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
    connect(m_cfgWidget, SIGNAL(CfgChanged(const CContextFreeGrammar&)), this, SLOT(CfgChangedSlot(const CContextFreeGrammar&)));
    connect(m_pdaWidget, SIGNAL(SignalPdaChanged(CPushDownAutomata*)), this, SLOT(PdaChangedSlot(CPushDownAutomata*)));

    m_pdaWidget->SetPda(new CPushDownAutomata());

    m_algorithmWidget->enableShowButton();
}

void CAlgorithmCFGtoPDA::InitInstructions()
{
    m_instructions.resize(END_INSTRUCTION);
    m_instructions[HEADER] =                                tr("<b>Input:</b> CFG <i>G</i>=(N, T, P, S)<br><b>Output:</b> PDA M'=(Q, " SIGMA ", " GAMA " , R, s, S, F)");
    m_instructions[SET_START_STATE] =                       "Q = {s}";
    m_instructions[SET_INPUT_ALPHABET] =                    SIGMA " = T";
    m_instructions[SET_STACK_ALPHABET] =                    tr(GAMA" = N " UNIFICATION " T");
    m_instructions[SET_PDA_RULES_HEADER] =                  tr("Construction of set R");
    m_instructions[FOREACH_PDA_RULES_FROM_INPUT_ALPHABET] = INDENT + tr("<b>for each</b> a " MEMBER_OF " " SIGMA);
    m_instructions[PDA_RULE_FROM_INPUT_ALPHABET] =          INDENT INDENT + tr("add asa → to R");
    m_instructions[FOREACH_PDA_RULES_FROM_CFG_RULES] =      INDENT + tr("<b>for each</b> A " ARROW " x " MEMBER_OF " P");
    m_instructions[SET_PDA_RULE_FROM_CFG_RULE] =            INDENT INDENT + tr("add As " ARROW " ys to R where y = reversal(x)");
    m_instructions[SET_FINITE_STATE] =                      tr("F = " EMPTYSET);
}

void CAlgorithmCFGtoPDA::RemoveFuture() {
	int count = m_history.count();
	for (int i = m_actPos + 1; i < count; i++)
	{
		m_history.removeLast();
	}
}

void CAlgorithmCFGtoPDA::NextStep()
{
	m_algorithmWidget->enablePrev();
	
	ComputeNextStep();
	
	m_pdaWidget->SetPda(new CPushDownAutomata(m_pda));
	RemoveFuture();
	if (m_actInstruction != lastInstruction)
	{
		SaveStep();
		if (m_breakpoints[m_actInstruction])
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

void CAlgorithmCFGtoPDA::ComputeNextStep()
{
    // TODO: Check if CFG is valid
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
                m_listOfAllTerminals = m_cfg.GetTerminalAlphabet().toList();
                m_actInstruction = FOREACH_PDA_RULES_FROM_INPUT_ALPHABET;
            }
            else if(m_cfg.GetRulesCount() > 0){
                m_listOfCfgRules = m_cfg.GetRules().toList();
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
            }
            else{
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        case FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
            m_actInstruction = PDA_RULE_FROM_INPUT_ALPHABET;
            break;
        case PDA_RULE_FROM_INPUT_ALPHABET:
            if(!m_listOfAllTerminals.isEmpty()){
                m_actInstruction = FOREACH_PDA_RULES_FROM_INPUT_ALPHABET;
            }
            else if(m_cfg.GetRulesCount() > 0){
                m_listOfCfgRules = m_cfg.GetRules().toList();
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
            }
            else{
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        case FOREACH_PDA_RULES_FROM_CFG_RULES:
            m_actInstruction = SET_PDA_RULE_FROM_CFG_RULE;
            break;
        case SET_PDA_RULE_FROM_CFG_RULE:
            if (!m_listOfCfgRules.isEmpty()) {
                m_actInstruction = FOREACH_PDA_RULES_FROM_CFG_RULES;
            }
            else {
                m_actInstruction = SET_FINITE_STATE;
            }
            break;
        default:
        case SET_FINITE_STATE:
            m_actInstruction = lastInstruction;
            break;
    }

    switch(m_actInstruction)
    {
        case SET_START_STATE:
        {
            m_pda.addState(START_STATE);
            m_pda.SetStartState(START_STATE);
            break;
        }
        case SET_INPUT_ALPHABET:
        {
            m_pda.SetAplhabet(CTerminal::CTerminalQSetToQStringQSet((m_cfg.GetTerminalAlphabet())));
            break;
        }
        case SET_STACK_ALPHABET:
        {
            m_pda.SetStackAlphabet(m_cfg.GetBothTerminalAndNonterminalAlphabet());
            break;
        }
        case FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
        {
            m_pdaActInputAplhabetSymbol = (QString) m_listOfAllTerminals.first();
            m_listOfAllTerminals.pop_front();
            break;
        }
        case PDA_RULE_FROM_INPUT_ALPHABET:
        {
            m_pda.AddPDARule(m_pda.GetStartState(), m_pda.GetStartState(), m_pdaActInputAplhabetSymbol, m_pdaActInputAplhabetSymbol, EPSILON);
            break;
        }
        case FOREACH_PDA_RULES_FROM_CFG_RULES:
        {
            m_actRule = m_listOfCfgRules.first();
            m_listOfCfgRules.pop_front();
            break;
        }
        case SET_PDA_RULE_FROM_CFG_RULE:
        {
            m_pda.AddPDARule(START_STATE, START_STATE, EPSILON, m_actRule.m_leftNonTerminal, m_actRule.GetRevertedRightRule());
            break;
        }
        case SET_FINITE_STATE:
        {
            break;
        }
        case END_INSTRUCTION:
        {
            break;
        }
    }

	if (m_actInstruction != lastInstruction)
	{
		m_prevInstruction = m_actInstruction;
	}
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
		case CAlgorithmCFGtoPDA::SET_START_STATE:
            return CHtmlCreator::JoinWithHtmlNewLines(
                {
                    m_pda.PrintHtmlStartState(),
                    m_pda.PrintHtmlStates()
                });
            break;
		case CAlgorithmCFGtoPDA::SET_INPUT_ALPHABET:
		{
            return CHtmlCreator::JoinWithHtmlNewLines(
                {
                    m_pda.PrintHtmlStartState(),
                    m_pda.PrintHtmlStates(),
                    m_pda.PrintHtmlAlphabet()
                });
			break;
		}
		case CAlgorithmCFGtoPDA::SET_STACK_ALPHABET:
            return CHtmlCreator::JoinWithHtmlNewLines(
                {
                    m_pda.PrintHtmlStartState(),
                    m_pda.PrintHtmlStates(),
                    m_pda.PrintHtmlAlphabet(),
                    m_pda.PrintHtmlStactAlphabet()
                });
			break;
        case CAlgorithmCFGtoPDA::FOREACH_PDA_RULES_FROM_INPUT_ALPHABET:
            return CHtmlCreator::JoinWithHtmlNewLines(
                {
                    m_pda.PrintHtmlStartState(),
                    m_pda.PrintHtmlStates(),
                    m_pda.PrintHtmlAlphabet(),
                    m_pda.PrintHtmlStactAlphabet(),
                    CHtmlCreator::PrintVariableValue("a", m_pdaActInputAplhabetSymbol),
                    m_pda.PrintHtmlPdaRules()
                });
            break;
        case CAlgorithmCFGtoPDA::PDA_RULE_FROM_INPUT_ALPHABET:
            return CHtmlCreator::JoinWithHtmlNewLines(
            {
                m_pda.PrintHtmlStartState(),
                m_pda.PrintHtmlStates(),
                m_pda.PrintHtmlAlphabet(),
                m_pda.PrintHtmlStactAlphabet(),
                CHtmlCreator::PrintVariableValue("a", m_pdaActInputAplhabetSymbol),
                m_pda.PrintHtmlPdaRules()
            });
            break;
        case CAlgorithmCFGtoPDA::FOREACH_PDA_RULES_FROM_CFG_RULES:
            return CHtmlCreator::JoinWithHtmlNewLines(
            {
                m_pda.PrintHtmlStartState(),
                m_pda.PrintHtmlStates(),
                m_pda.PrintHtmlAlphabet(),
                m_pda.PrintHtmlStactAlphabet(),
                CHtmlCreator::PrintVariableValue("act_rule", m_actRule.ToHtml()),
                m_pda.PrintHtmlPdaRules()
            });
            break;
        case CAlgorithmCFGtoPDA::SET_PDA_RULE_FROM_CFG_RULE:
            return CHtmlCreator::JoinWithHtmlNewLines(
            {
                m_pda.PrintHtmlStartState(),
                m_pda.PrintHtmlStates(),
                m_pda.PrintHtmlAlphabet(),
                m_pda.PrintHtmlStactAlphabet(),
                CHtmlCreator::PrintVariableValue("act_rule", m_actRule.ToHtml()),
                m_pda.PrintHtmlPdaRules()
            });
            break;
        default:
        case CAlgorithmCFGtoPDA::SET_FINITE_STATE:
            return CHtmlCreator::JoinWithHtmlNewLines(
            {
                m_pda.PrintHtmlStartState(),
                m_pda.PrintHtmlStates(),
                m_pda.PrintHtmlAlphabet(),
                m_pda.PrintHtmlStactAlphabet(),
                m_pda.PrintHtmlPdaRules(),
                m_pda.PrintHtmlFinalStates()
            });
            break;
    }
    return QString();
}

void CAlgorithmCFGtoPDA::SetInputCfg(CContextFreeGrammar *cfg)
{
    m_cfg = *cfg;
    m_cfgWidget->SetCfg(*cfg);
    m_pdaWidget->clearStatus();
}

void CAlgorithmCFGtoPDA::SetInputPda(CPushDownAutomata *pda)
{
    m_pdaWidget->SetPda(pda);
}

CContextFreeGrammar CAlgorithmCFGtoPDA::GetCfg()
{
    return m_cfgWidget->GetCfg();
}

CPushDownAutomata CAlgorithmCFGtoPDA::GetPda()
{
    return m_pdaWidget->GetPda();
}

void CAlgorithmCFGtoPDA::SetMode(AlgorithmModes mode)
{
    m_mode = mode;
    ResetAlgorithm();
}

void CAlgorithmCFGtoPDA::PrevStep()
{
	if (m_actPos > 0)
	{
		m_algorithmWidget->enableNext();
		m_actPos--;
		SStep s = m_history.at(m_actPos);

		m_num = s.m_num;
		m_actInstruction = s.m_actInstruction;
		m_prevInstruction = s.m_prevInstruction;
		m_pda = s.m_pda;
		m_userPda = s.m_userPda;
		m_cfg = s.m_cfg;
		m_pdaActInputAplhabetSymbol = s.m_pdaActInputAplhabetSymbol;
		m_actRule = s.m_actRule;
		m_listOfCfgRules = s.m_listOfCfgRules;
		m_listOfAllTerminals = s.m_listOfAllTerminals;
		m_pdaWidget->SetPda(new CPushDownAutomata(m_pda));

		ShowVariables();
		SetActInstruction();
		m_pdaWidget->clearStatus();
	}
	else
	{
		m_algorithmWidget->disablePrev();
	}
}

void CAlgorithmCFGtoPDA::ComputeCorrectSolution()
{
    CPushDownAutomata tmpPda = m_pda;
	while (m_actInstruction != lastInstruction) {
		ComputeNextStep();
	}
	m_correctPda = m_pda;
	m_pda = tmpPda;
}

void CAlgorithmCFGtoPDA::CheckSolution()
{
	if (CPushDownAutomata::AreEquivalent(m_userPda, m_correctPda))
	{
		m_pdaWidget->setCorrectStatus();
	}
    else
	{
		m_pdaWidget->setWrongStatus();
	}
}

void CAlgorithmCFGtoPDA::ShowCorrectSolution()
{
	m_backupUserPda = m_userPda;
	m_pdaWidget->SetPda(new CPushDownAutomata(m_correctPda));
}

void CAlgorithmCFGtoPDA::ShowUserSolution()
{
	m_pdaWidget->SetPda(new CPushDownAutomata(m_backupUserPda));
}

void CAlgorithmCFGtoPDA::ToBegin()
{
	m_algorithmWidget->enableNext();
	m_actPos = 0;
	SStep s = m_history.at(m_actPos);

	m_num = s.m_num;
	m_actInstruction = s.m_actInstruction;
	m_prevInstruction = s.m_prevInstruction;
	m_pda = s.m_pda;
	m_userPda = s.m_userPda;
	m_cfg = s.m_cfg;
	m_pdaActInputAplhabetSymbol = s.m_pdaActInputAplhabetSymbol;
	m_actRule = s.m_actRule;
	m_listOfCfgRules = s.m_listOfCfgRules;
	m_listOfAllTerminals = s.m_listOfAllTerminals;
	m_pdaWidget->SetPda(new CPushDownAutomata(m_pda));

	ShowVariables();
	SetActInstruction();
	m_pdaWidget->clearStatus();
	m_algorithmWidget->disablePrev();
}

void CAlgorithmCFGtoPDA::ToEnd()
{
    while(m_actInstruction != lastInstruction)
        NextStep();
}

void CAlgorithmCFGtoPDA::CfgChangedSlot(const CContextFreeGrammar& cfg)
{
    m_cfg = cfg;
    m_pdaWidget->clearStatus();
    ResetAlgorithm();
}

void CAlgorithmCFGtoPDA::PdaChangedSlot(CPushDownAutomata *pda)
{
    m_userPda = *pda;
}

void CAlgorithmCFGtoPDA::ResetAlgorithm()
{
    m_playTimer->stop();
    m_num = 0;
    m_actInstruction=HEADER;
    m_prevInstruction=HEADER;

    ClearActInstruction();
    m_variablesWidget->SetText("");
    // because show correct solution break connectiom betwen user FA and user FA widget
    connect(m_pdaWidget,SIGNAL(SignalPdaChanged(CPushDownAutomata* pda)),this,SLOT(PdaChangedSlot(CPushDownAutomata*)));

    switch (m_mode)
    {
        case algorithmSteping:
            m_algorithmWidget->enableNext();
            m_algorithmWidget->disablePrev();
            m_pdaWidget->SetPda(new CPushDownAutomata());
            //unselect instruction from algorithm window
            ClearActInstruction();
            m_history.clear();
            m_actPos = 0;
            m_actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            SaveStep();
            m_CheckStepTimer->stop();
            break;
        case individualWork:
            ComputeCorrectSolution();
            m_CheckStepTimer->stop();
            break;
        case instantChecking:
            ComputeCorrectSolution();
            m_CheckStepTimer->start(CHECK_STEP_TIMEOUT);
            break;
        case NONE:
            m_CheckStepTimer->stop();
            break;
    }

    m_pdaWidget->clearStatus();
}

void CAlgorithmCFGtoPDA::SaveStep(){
    SStep s;
    s.m_num = ++m_num;
    s.m_actInstruction = m_actInstruction;
    s.m_prevInstruction = m_prevInstruction;
    s.m_pda = m_pda;
    s.m_userPda = m_userPda;
    s.m_cfg = m_cfg;
    s.m_pdaActInputAplhabetSymbol = m_pdaActInputAplhabetSymbol;
    s.m_actRule = m_actRule;
    s.m_listOfCfgRules = m_listOfCfgRules;
    s.m_listOfAllTerminals = m_listOfAllTerminals;
    
	m_history.push_back(s);
	m_actPos = m_history.count() - 1;
}
