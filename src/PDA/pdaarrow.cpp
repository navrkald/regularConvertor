#include "pdaarrow.h"
#include <PDA/pdaruledialog.h>

CPdaArrow::CPdaArrow(StateNode* startItem,
										 StateNode* endItem,
										 CPushDownAutomata* pda,
										 QSet<CPDACompotutationalRule> pdaRules,
										 QGraphicsItem* parent,
										 CPDADiagramScene * pdaScene):
	Arrow(startItem, endItem, parent, pdaScene),
	m_pda(pda),
	m_pdaRules(pdaRules),
	m_pdaScene(pdaScene)
{
	m_displayText = CPDACompotutationalRule::ToArrowText(pdaRules);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	m_myColor = Qt::black;
	setPen(QPen(m_myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	m_debugCounter = 0;

	connect(this, SIGNAL(SignalPdaChanged(CPushDownAutomata*)), m_pdaScene, SIGNAL(SetPdaSlot(CPushDownAutomata* pda)));
}

bool CPdaArrow::RemoveRule(const CPDACompotutationalRule& rule)
{
	m_pdaRules.remove(rule);
	update();
	return m_pdaRules.isEmpty();
}

QSet<CPDACompotutationalRule> CPdaArrow::GetPdaRules()
{
	return m_pdaRules;
}

void CPdaArrow::EditArrow()
{
	CPdaRuleDialog inputDialog(m_myStartItem->getName(), m_myEndItem->getName(), m_pdaRules);
	if(QDialog::Accepted == inputDialog.exec()){
		QSet<CPDACompotutationalRule> newPdaRules = inputDialog.GetPdaRules();
		QSet<CPDACompotutationalRule> addedRules  = newPdaRules - m_pdaRules;
		QSet<CPDACompotutationalRule> deletedRules  =  m_pdaRules - newPdaRules;
		m_pda->AddPdaRules(addedRules);
		m_pda->RemovePdaRules(deletedRules);
		m_displayText = CPDACompotutationalRule::ToArrowText(m_pda->GetRules());
	}

	emit SignalPdaChanged(m_pda);
}

