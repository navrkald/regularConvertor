#ifndef CPDAARROR_H
#define CPDAARROR_H

#include <QObject>
#include <finite_machine/arrow.h>
#include <finite_machine/pushdownautomata.h>
#include <PDA/pdadiagramscene.h>

class CPdaArrow : public Arrow
{
	Q_OBJECT
public:
	explicit CPdaArrow();
	CPdaArrow(StateNode *startItem, StateNode *endItem, CPushDownAutomata* pa, QSet<CPDACompotutationalRule> pdaRules,
	QGraphicsItem *parent = 0, CPDADiagramScene *scene = 0);

protected:
	StateNode* m_startItem;
	StateNode* m_endItem;
	CPushDownAutomata* m_pa;
	QSet<CPDACompotutationalRule> m_oldPdaRules;
	QGraphicsItem* m_parent;
	CPDADiagramScene* m_pdaScene;
	virtual void EditArrow();

signals:

public slots:
};

#endif // CPDAARROR_H
