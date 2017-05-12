#include "pdacompotutationalrules.h"
#include <QtCore>

// TODO: Write unit tests for this method
CPDACompotutationalRule::CPDACompotutationalRule(QString rule)
{
    const QRegExp ruleRegExp("^(\\w+)\\s+(\\w)\\s+(\\w)\\s*->\\s*(\\w)\\s+(\\w+)$");
    if(ruleRegExp.exactMatch(rule))
    {
        ruleRegExp.indexIn(rule);
        from = ruleRegExp.cap(2);
        to = ruleRegExp.cap(5);
        symbol = ruleRegExp.cap(3);
        QString pushSymbolsStr = ruleRegExp.cap(4);
        QStringList pushSymbolsStrList =  pushSymbolsStr.split(",");
        foreach (QString pushSymbol, pushSymbolsStrList)
        {
            m_pushSymbols.append(pushSymbol.trimmed());
        }

        m_popSymol = ruleRegExp.cap(1);
		}
}

QString CPDACompotutationalRule::ToArrowText(const QSet<CPDACompotutationalRule>& pdaRules)
{
	QString outText;
	foreach(CPDACompotutationalRule r, pdaRules){
		outText.append(r.GetPopSymbol() + ", " + r.GetSymbol() + " -> " + r.GetPushSymbols().toList().join(", ") + "\n");
	}
	return outText;
}

QDataStream & operator<<(QDataStream & out, const CPDACompotutationalRule& r)
{
	
	return r.WriteToQDataStream(out);	
}

QDataStream & operator>>(QDataStream & in, CPDACompotutationalRule& r)
{
	return r.ReadFromQDataStream(in);
}

QDataStream & CPDACompotutationalRule::WriteToQDataStream(QDataStream & out) const
{
	ComputationalRules::WriteToQDataStream(out);
	return out << m_popSymol << m_pushSymbols;
}

QDataStream & CPDACompotutationalRule::ReadFromQDataStream(QDataStream & in)
{
	ComputationalRules::ReadFromQDataStream(in);
	return in >> m_popSymol >> m_pushSymbols;
}

QSet<QString> CPDACompotutationalRule::GetStackSymbols()
{
	QSet<QString> stackSymbols;
	stackSymbols.insert(m_popSymol);
	stackSymbols += m_pushSymbols.toList().toSet();
	return stackSymbols;
}

QSet<QString> PdaComputationalRulesToQSetOfStrings(QSet<CPDACompotutationalRule> rules)
{
	QSet<QString> out;
	foreach(CPDACompotutationalRule rule, rules) {
        out.insert(rule.ToString());
	}
	return out;
}

QString PdaComputationalRulesToString(QSet<CPDACompotutationalRule> rules)
{
	QString outStr;
	foreach(CPDACompotutationalRule rule, rules) {
		outStr += rule.ToString() + "\n";
	}
	return outStr;
}
