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
