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
	CPdaArrow(StateNode *startItem, StateNode *endItem, CPushDownAutomata* pda, QSet<CPDACompotutationalRule> pdaRules,
	QGraphicsItem *parent = 0, CPDADiagramScene *scene = 0);
	bool RemoveRule(const CPDACompotutationalRule& rule);
	QSet<CPDACompotutationalRule> GetPdaRules();
    void AddPdaRule(const CPDACompotutationalRule& rule);

protected:
	StateNode* m_startItem;
	StateNode* m_endItem;
	CPushDownAutomata* m_pda;
	QSet<CPDACompotutationalRule> m_pdaRules;
	QGraphicsItem* m_parent;
	CPDADiagramScene* m_pdaScene;
	virtual void EditArrow();

signals:
	void SignalPdaChanged(CPushDownAutomata* pda);

public slots:
};

#endif // CPDAARROR_H
