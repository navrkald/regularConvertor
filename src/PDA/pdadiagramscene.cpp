#include "pdadiagramscene.h"
#include <PDA/pdaarrow.h>
#include <PDA/pdaruledialog.h>


CPDADiagramScene::CPDADiagramScene(CPushDownAutomata* pa, QWidget *parent) :
	DiagramScene(pa, parent),
	m_pda(pa) 
{}


void CPDADiagramScene::AddEdgesSlot(QSet<CPDACompotutationalRule> pdaRules)
{
	foreach(CPDACompotutationalRule rule, pdaRules)
	{
		StateNode* from = getNodeByName(rule.from);
		StateNode* to = getNodeByName(rule.to);
		QString symbol = rule.symbol;
		Arrow* arrow = getArrow(from, to);
		if (arrow == NULL)
		{
			QStringList symbolList;
			symbolList.append(symbol);
			CPdaArrow* newArrow = new CPdaArrow(from, to, m_pda, QSet<CPDACompotutationalRule>{ rule }, 0, this);
			from->addArrow(newArrow);
			to->addArrow(newArrow);
			newArrow->setZValue(-1000.0);   //posun na pozadi
			addItem(newArrow);
			newArrow->updatePosition();
		}
		else
		{
			arrow->addSymbol(symbol);
			arrow->updatePosition();
		}
		m_pda->AddPDARule(rule);
		//m_fa->addRule(rule);
	}
	EmitFiniteAutomataChanged();
}

void CPDADiagramScene::AddNewState(QString nodeName, QPoint position)
{
	if (nodeName.isEmpty()) {
		nodeName = CreateNodeUniqueName();
	}
	m_pda->addState(nodeName);
	if (!position.isNull())
		SetNodeCoordinates(nodeName, position);
	EmitFiniteAutomataChanged();
}

void CPDADiagramScene::SetStartState(QString nodeName, bool emitFaChanged)
{
	m_pda->SetStartState(nodeName);
	if (emitFaChanged)
		EmitFiniteAutomataChanged();
}

void CPDADiagramScene::SetEndingState(QString nodeName, bool isEnding)
{
	//setup FA
	if (isEnding)
		m_pda->addFinalState(nodeName);
	else
		m_pda->removeFinalState(nodeName);

	EmitFiniteAutomataChanged();
}

void CPDADiagramScene::SetNodeCoordinates(QString nodeName, QPoint position)
{
	m_pda->m_coordinates[nodeName] = position;
}

bool CPDADiagramScene::CanSetNodeName(QString newName)
{
	return m_pda->isStateUnique(newName);
}

void CPDADiagramScene::RenameNode(QString oldName, QString newName)
{
	if (CanSetNodeName(newName)) {
		m_pda->renameState(oldName, newName);
		EmitFiniteAutomataChanged();
	}
}

void CPDADiagramScene::RemoveEdgesSlot(QSet<CPDACompotutationalRule> pdaRules)
{
	CPDACompotutationalRule rule;
	foreach(rule, pdaRules)
	{
		StateNode* from = getNodeByName(rule.from);
		StateNode* to = getNodeByName(rule.to);
		QString symbol = rule.symbol;
		CPdaArrow* arrow = dynamic_cast<CPdaArrow*> (getArrow(from, to));
		if (arrow && arrow->RemoveRule(rule))
		{//remove arrow
			delete arrow;
		}
	}
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

void CPDADiagramScene::CheckArrowTypeAndDelete(QGraphicsItem * graphicsItem, QList<QGraphicsItem*>& items)
{
	CPdaArrow* arrow = dynamic_cast<CPdaArrow*>(graphicsItem);
	if (arrow)
	{
		removeItem(arrow);
		items.removeOne(arrow);
		m_pda->RemovePdaRules(arrow->GetPdaRules());
		delete arrow;
	}
}

void CPDADiagramScene::EmitFiniteAutomataChanged()
{
	emit PdaChangedSignal(m_pda);
}

QString CPDADiagramScene::CreateNodeUniqueName()
{
	return m_pda->createUniqueName();
}

void CPDADiagramScene::RemoveStateFromFiniteAutomata(QString state)
{
	m_pda->removeState(state);
}
