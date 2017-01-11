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
	void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QVector<QString> pushSymbols);
	void AddPDARule(QString from, QString to, QString symbol, QString popSymol, QString pushSymbol);
	void AddPdaRules(QSet<CPDACompotutationalRule> rulesToAdd);
	void RemovePdaRules(QSet<CPDACompotutationalRule> rulesToDelete);
	QSet<CPDACompotutationalRule> GetRulesWithInputSymbol(const QString& inputSymbol);
	QSet<CPDACompotutationalRule> GetRulesWithStackSymbol(const QString& stackSymbol);
public:
	QSet <QString> m_stackAlphabet;
	QSet <CPDACompotutationalRule> m_pdaRules;
};

#endif // PUSHDOWNAUTOMATA_H
