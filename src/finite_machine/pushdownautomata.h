#ifndef PUSHDOWNAUTOMATA_H
#define PUSHDOWNAUTOMATA_H

#include "finiteautomata.h"
#include "pdacompotutationalrules.h"

class CPushDownAutomata : public FiniteAutomata
{
public:
	CPushDownAutomata();
	QSet <CPDACompotutationalRule> GetRules() const { return m_pdaRules; }
	void SetStackAlphabet(QSet<QString> stackAplhabet);
	void AddStactSymbols(QSet<QString> stackAplhabet);
	void AddPDARule(CPDACompotutationalRule rule);
	void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QVector<QString> pushSymbols);
	void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QString pushSymbol);
	void AddPdaRules(QSet<CPDACompotutationalRule> rulesToAdd);
	void RemovePDARule(CPDACompotutationalRule rule);
	void RemovePdaRules(QSet<CPDACompotutationalRule> rulesToDelete);
    QSet<CPDACompotutationalRule> GetRulesWithInputSymbol(const QString& inputSymbol);
	QSet<CPDACompotutationalRule> GetRulesWithStackSymbol(const QString& stackSymbol);

	QString PrintHtmlPdaRules();
	QString PrintHtmlStactAlphabet();

	bool RenameState(QString oldStateName, QString newStateName);
	static bool AreEquivalent(CPushDownAutomata pda1, CPushDownAutomata pda2);
	static bool AreEquivalentSimpleImplementation(CPushDownAutomata pda1, CPushDownAutomata pda2);
	QDataStream& operator<<(QDataStream &out);
	QDataStream& operator >> (QDataStream &in);

public:
	QSet <QString> m_stackAlphabet;
	QSet <CPDACompotutationalRule> m_pdaRules;
};

bool operator==(const CPushDownAutomata& pda1, const CPushDownAutomata& pda2);
QDataStream& operator<<(QDataStream &out, const FiniteAutomata &FA);
QDataStream& operator >> (QDataStream &in, FiniteAutomata &FA);

#endif // PUSHDOWNAUTOMATA_H
