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
	m_displayText = CPDACompotutationalRule::ToArrowText(pdaRules);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	m_myColor = Qt::black;
	setPen(QPen(m_myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	m_debugCounter = 0;

	// TODO connect to scene
}

void CPdaArrow::EditArrow()
{
	CPdaRuleDialog inputDialog(m_myStartItem->getName(), m_myEndItem->getName(), m_oldPdaRules);
	if(QDialog::Accepted == inputDialog.exec()){
		QSet<CPDACompotutationalRule> newPdaRules = inputDialog.GetPdaRules();
		QSet<CPDACompotutationalRule> addedRules  = newPdaRules - m_oldPdaRules;
		QSet<CPDACompotutationalRule> deletedRules  =  m_oldPdaRules - newPdaRules;
		m_pa->AddPdaRules(addedRules);
		m_pa->RemovePdaRules(deletedRules);
		m_displayText = CPDACompotutationalRule::ToArrowText(m_pa->GetRules());
	}

	// TODO emit that PDA has changed
}

