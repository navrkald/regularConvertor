#include "computationalrules.h"

ComputationalRules::ComputationalRules()
{

}

ComputationalRules::ComputationalRules(QString _from, QString _to, QString _symbol)
{
    from = _from;
    to = _to;
    symbol= _symbol;
}

ComputationalRules::ComputationalRules(QString rule)
{
    QRegExp ruleRegExp("^(\\w+)\\s+(\\w)\\s*->\\s*(\\w+)$");
    if(ruleRegExp.exactMatch(rule))
    {
        ruleRegExp.indexIn(rule);
        from = ruleRegExp.cap(1);
        symbol = ruleRegExp.cap(2);
        to = ruleRegExp.cap(3);
        qDebug() << "From= " <<from << "\nSymbol= " << symbol << "\nto= " <<to;
    }
}

ComputationalRules::ComputationalRules(const ComputationalRules& object)
{
    this->from = object.from;
    this->to = object.to;
    this->symbol = object.symbol;
}

uint qHash(const ComputationalRules &rule)
{
    //TODO vylepsit tento hash
    return 1;
}
