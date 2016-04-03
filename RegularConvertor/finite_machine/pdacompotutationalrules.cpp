#include "pdacompotutationalrules.h"

CPDACompotutationalRule::CPDACompotutationalRule(QString rule)
{
    const QRegExp ruleRegExp("^(\\w+)\\s+(\\w)\\s+(\\w)\\s*->\\s*(\\w)\\s+(\\w+)$");
    if(ruleRegExp.exactMatch(rule))
    {
        ruleRegExp.indexIn(rule);
        from = ruleRegExp.cap(2);
        to = ruleRegExp.cap(5);
        symbol = ruleRegExp.cap(3);
        pushSymbol = ruleRegExp.cap(4);
        popSymol = ruleRegExp.cap(1);
    }
}
