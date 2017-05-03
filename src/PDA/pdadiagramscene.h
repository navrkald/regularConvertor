#ifndef CPDADIAGRAMSCENE_H
#define CPDADIAGRAMSCENE_H

#include <finite_machine/diagramscene.h>
#include <finite_machine/pushdownautomata.h>

class CPDADiagramScene : public DiagramScene
{
  Q_OBJECT
public:
	CPDADiagramScene(CPushDownAutomata* pa, QWidget *parent);

public:
	virtual void AddNewState(QString nodeName, QPoint position = QPoint()) override;
	virtual void SetStartState(QString nodeName, bool emitFaChanged = true) override;
	virtual void SetEndingState(QString nodeName, bool isEnding) override;
	virtual void SetNodeCoordinates(QString nodeName, QPoint position) override;
	virtual bool CanSetNodeName(QString newName) override;
	virtual void RenameNode(QString oldName, QString newName) override;

public slots:
	void AddEdgesSlot(QSet<CPDACompotutationalRule> pdaRules);
	void RemoveEdgesSlot(QSet<CPDACompotutationalRule> pdaRules);
	void SetPdaSlot(CPushDownAutomata* pda);

signals:
	void PdaChangedSignal(CPushDownAutomata*);

protected:
	CPushDownAutomata* m_pda;

protected:
	virtual void AddArrow(StateNode *startItem, StateNode *endItem) override;
	virtual void RemoveStateFromFiniteAutomata(QString state) override;
	virtual void CheckArrowTypeAndDelete(QGraphicsItem* graphicsItem, QList<QGraphicsItem*>& items) override;
	virtual void EmitFiniteAutomataChanged() override;
	virtual QString CreateNodeUniqueName() override;
};

#endif // CPDADIAGRAMSCENE_H
