#include "pdadiagramscene.h"
#include <PDA/pdaarrow.h>
#include <PDA/pdaruledialog.h>

void CPDADiagramScene::AddEdgesSlot(QSet<CPDACompotutationalRule> pdaRules)
{
    // TODO: Implement
}

void CPDADiagramScene::RemoveEdgesSlot(QSet<CPDACompotutationalRule> pdaRules)
{
    // TODO: Implement
}

void CPDADiagramScene::SetPdaSlot(CPushDownAutomata* pda)
{
    clean();
    clear();
    clearFocus();
    clearSelection();
    m_pda = pda;
    AddNodes(m_pda->m_states, m_pda->m_coordinates);
    SetStartNode(m_pda->m_startState);
    AddEdgesSlot(m_pda->m_pdaRules);
    AddEndingNodes(m_pda->m_finalStates);
    emit PdaChangedSignal(m_pda);
}

void CPDADiagramScene::AddArrow(StateNode *startItem, StateNode *endItem){
	CPdaRuleDialog inputDialog(startItem->getName(), endItem->getName(), QSet<CPDACompotutationalRule>());
	if(QDialog::Accepted == inputDialog.exec())
	{
		QSet<CPDACompotutationalRule> rules = inputDialog.GetPdaRules();
        if(!rules.isEmpty()){
            CPdaArrow* arrow = new CPdaArrow(startItem, endItem, m_pda, rules, 0, this);
            m_pda->AddPdaRules(rules);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);   //posun na pozadi
            addItem(arrow);
            arrow->updatePosition();
            emit PdaChangedSignal(m_pda);
        }
	}
}
