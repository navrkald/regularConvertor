#include "pushdownautomata.h"

CPushDownAutomata::CPushDownAutomata()
{
}

void CPushDownAutomata::SetStackAlphabet(QSet<QString> stackAplhabet)
{
    m_stackAlphabet = stackAplhabet;
}

void CPushDownAutomata::AddPDARule(QString from, QString to, QString symbol, QString popSymol, QVector<QString> pushSymbols)
{
    CPDACompotutationalRule newPDARule(from, to, symbol, popSymol, pushSymbols);
    m_pdaRules.insert(newPDARule);
}

void CPushDownAutomata::AddPDARule(QString from, QString to, QString symbol, QString popSymol, QString pushSymbol)
{
    CPDACompotutationalRule newPDARule(from, to, symbol, popSymol, pushSymbol);
		m_pdaRules.insert(newPDARule);
}

void CPushDownAutomata::AddPdaRules(QSet<CPDACompotutationalRule> rulesToAdd)
{
	QSet<QString> inputSymbols;
	QSet<QString> stackSymbols;

	// Add and collect used stack and input symbols
	foreach(CPDACompotutationalRule r, rulesToAdd){
		m_pdaRules.insert(r);
		inputSymbols.insert(r.GetSymbol());
		stackSymbols += r.GetPopSymbol();
		stackSymbols += r.GetPushSymbols().toList().toSet();
	}

	// Add stack and input symbols
	alphabet += inputSymbols;
	m_stackAlphabet += stackSymbols;
}

void CPushDownAutomata::RemovePdaRules(QSet<CPDACompotutationalRule> rulesToDelete)
{
	QSet<QString> inputSymbols;
	QSet<QString> stackSymbols;

	// Remove rules and collect unsed stack and input symbols
	foreach(CPDACompotutationalRule r, rulesToDelete){
		m_pdaRules.remove(r);
		inputSymbols.insert(r.GetSymbol());
		stackSymbols += r.GetPopSymbol();
		stackSymbols += r.GetPushSymbols().toList().toSet();
	}

	// Remove unused input symbols
	foreach(QString inputSymbol, inputSymbols){
		if(GetRulesWithInputSymbol(inputSymbol).isEmpty()){
			alphabet.remove(inputSymbol);
		}
	}

	// Remove unused stack symbols
	foreach(QString stackSymbol, stackSymbols){
		if(GetRulesWithStackSymbol(stackSymbol).isEmpty()){
			m_stackAlphabet.remove(stackSymbol);
		}
	}
}

QSet<CPDACompotutationalRule> CPushDownAutomata::GetRulesWithInputSymbol(const QString& inputSymbol)
{
	QSet<CPDACompotutationalRule> findedRules;
	foreach(CPDACompotutationalRule r,m_pdaRules){
		if(r.GetSymbol() == inputSymbol){
			findedRules.insert(r);
		}
	}
	return findedRules;
}

QSet<CPDACompotutationalRule> CPushDownAutomata::GetRulesWithStackSymbol(const QString& stackSymbol)
{
	QSet<CPDACompotutationalRule> findedRules;
	foreach(CPDACompotutationalRule r,m_pdaRules){
		if(r.GetStackSymbols().contains(stackSymbol) ){
			findedRules.insert(r);
		}
	}
	return findedRules;
}
