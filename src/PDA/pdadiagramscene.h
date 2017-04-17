#ifndef CPDADIAGRAMSCENE_H
#define CPDADIAGRAMSCENE_H

#include <finite_machine/diagramscene.h>
#include <finite_machine/pushdownautomata.h>

class CPDADiagramScene : public DiagramScene
{
  Q_OBJECT
public:
  CPDADiagramScene(CPushDownAutomata* pa, QWidget *parent) :
    DiagramScene(pa, parent),
    m_pa(pa) {}

public slots:
    void AddEdgesSlot(QSet<CPDACompotutationalRule> pdaRules);
    void RemoveEdgesSlot(QSet<CPDACompotutationalRule> pdaRules);
    void SetPdaSlot(QSet<CPDACompotutationalRule> pdaRules);

signals:
    void PdaChangedSignal(CPushDownAutomata* pda);

protected:
  CPushDownAutomata* m_pa;

protected:
	virtual void AddArrow(StateNode *startItem, StateNode *endItem);
};

#endif // CPDADIAGRAMSCENE_H
