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

void CPDADiagramScene::SetPdaSlot(QSet<CPDACompotutationalRule> pdaRules)
{
    // TODO: Implement
}

void CPDADiagramScene::AddArrow(StateNode *startItem, StateNode *endItem){
	CPdaRuleDialog inputDialog(startItem->getName(), endItem->getName(), QSet<CPDACompotutationalRule>());
	if(QDialog::Accepted == inputDialog.exec())
	{
		QSet<CPDACompotutationalRule> rules = inputDialog.GetPdaRules();
        if(!rules.isEmpty()){
            CPdaArrow* arrow = new CPdaArrow(startItem, endItem, m_pa, rules, 0, this);
            m_pa->AddPdaRules(rules);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);   //posun na pozadi
            addItem(arrow);
            arrow->updatePosition();
            // TODO: Emit FA changed // 						 emit FA_changed(FA);
        }
	}
}
