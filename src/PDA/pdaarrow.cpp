#include "pdaarrow.h"
#include <PDA/pdaruledialog.h>

CPdaArrow::CPdaArrow(StateNode* startItem,
										 StateNode* endItem,
										 CPushDownAutomata* pa,
										 QSet<CPDACompotutationalRule> pdaRules,
										 QGraphicsItem* parent,
										 CPDADiagramScene * pdaScene):
	Arrow(startItem, endItem, parent, pdaScene),
	m_pa(pa),
	m_oldPdaRules(pdaRules),
	m_pdaScene(pdaScene)
{
	displayText = CPDACompotutationalRule::ToArrowText(pdaRules);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor = Qt::black;
	setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	debugCounter = 0;

	// TODO connect to scene
}

void CPdaArrow::EditArrow()
{
	CPdaRuleDialog inputDialog(myStartItem->getName(), myEndItem->getName(), m_oldPdaRules);
	if(QDialog::Accepted == inputDialog.exec()){
		QSet<CPDACompotutationalRule> newPdaRules = inputDialog.GetPdaRules();
		QSet<CPDACompotutationalRule> addedRules  = newPdaRules - m_oldPdaRules;
		QSet<CPDACompotutationalRule> deletedRules  =  m_oldPdaRules - newPdaRules;
		m_pa->AddPdaRules(addedRules);
		m_pa->RemovePdaRules(deletedRules);
		displayText = CPDACompotutationalRule::ToArrowText(m_pa->GetRules());
	}

	// TODO emit that PDA has changed
}

