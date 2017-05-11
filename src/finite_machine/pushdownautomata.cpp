#include "pushdownautomata.h"
#include <algorithms/constants.h>
#include <htmlcreator.h>

CPushDownAutomata::CPushDownAutomata()
{
}

void CPushDownAutomata::SetStackAlphabet(QSet<QString> stackAplhabet)
{
    m_stackAlphabet = stackAplhabet;
	m_stackAlphabet.remove(EPSILON);
}

void CPushDownAutomata::AddStactSymbols(QSet<QString> stackAplhabet) {
	m_stackAlphabet += stackAplhabet;
	m_stackAlphabet.remove(EPSILON);
}

void CPushDownAutomata::AddPDARule(CPDACompotutationalRule rule)
{
	m_pdaRules.insert(rule);
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
	AddAplhabet(inputSymbols);
	AddStactSymbols(stackSymbols);
}

void CPushDownAutomata::RemovePDARule(CPDACompotutationalRule rule)
{
	m_pdaRules.remove(rule);
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
			m_alphabet.remove(inputSymbol);
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

QString CPushDownAutomata::PrintHtmlPdaRules()
{
    return CHtmlCreator::PrintHtmlSet("R", PdaComputationalRulesToQSetOfStrings((m_pdaRules)), HTMLNEWLINE INDENT);
}

QString CPushDownAutomata::PrintHtmlStactAlphabet()
{
    return CHtmlCreator::PrintHtmlSet(QString::fromUtf8(GAMA), m_stackAlphabet);
}

bool CPushDownAutomata::AreEquivalent(CPushDownAutomata pda1, CPushDownAutomata pda2)
{
	return AreEquivalentSimpleImplementation(pda1, pda2);
}

bool CPushDownAutomata::AreEquivalentSimpleImplementation(CPushDownAutomata pda1, CPushDownAutomata pda2)
{
	if (pda1.m_states.size() == 1 && pda1.m_states.size() == pda2.m_states.size()) {
		pda1.RenameState(pda1.m_startState, "1");
		pda2.RenameState(pda2.m_startState, "1");
	}
	
	return pda1 == pda2;
	
}

QDataStream& CPushDownAutomata::operator<<(QDataStream & out)
{
	FiniteAutomata::operator<<(out);
	return out << m_stackAlphabet << m_pdaRules;
}

QDataStream& CPushDownAutomata::operator>>(QDataStream & in)
{
	FiniteAutomata::operator>>(in);
	return in >> m_stackAlphabet >> m_pdaRules;
}

bool operator ==(const CPushDownAutomata& pda1, const CPushDownAutomata& pda2)
{
	if (pda1.m_states != pda2.m_states)
		return false;

	if (pda1.m_alphabet != pda2.m_alphabet)
		return false;

	if (pda1.m_stackAlphabet != pda2.m_stackAlphabet)
		return false;

	if (pda1.m_startState != pda2.m_startState)
		return false;

	if (pda1.m_finalStates != pda2.m_finalStates)
		return false;

	if (pda1.m_pdaRules != pda2.m_pdaRules)
		return false;

	qDebug() << "User rules: " << PdaComputationalRulesToString(pda1.m_pdaRules) << "\n";
	qDebug() << "Corr rules: " << PdaComputationalRulesToString(pda2.m_pdaRules) << "\n";

	return true;
}

bool CPushDownAutomata::RenameState(QString oldStateName, QString newStateName) {
	foreach(CPDACompotutationalRule pdaRule, m_pdaRules) {
		if (pdaRule.from == oldStateName || pdaRule.to == oldStateName)
		{
			CPDACompotutationalRule newRule = pdaRule;
			if (pdaRule.from == oldStateName)
				newRule.from = newStateName;
			if (pdaRule.to == oldStateName)
				newRule.to = newStateName;
			this->AddPDARule(newRule);
			this->RemovePDARule(pdaRule);
		}
	}
	return FiniteAutomata::RenameState(oldStateName, newStateName);
}