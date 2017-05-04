#include "pdacompotutationalrules.h"

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
