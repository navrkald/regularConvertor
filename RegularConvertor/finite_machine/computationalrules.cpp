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



bool lessThan(const ComputationalRules& r1, const ComputationalRules& r2)
{
    QString s1 = r1.toString();
    QString s2 = r2.toString();
    bool return_val = s1 < s2;
    return  return_val;
}

bool operator<(const ComputationalRules& r1, const ComputationalRules& r2)
{
    return lessThan(r1,r2);
}

QDebug operator<<(QDebug dbg, const ComputationalRules& rule)
{

    dbg.nospace() << rule.toString()+'\n';
    return dbg.maybeSpace();
}

uint qHash(const ComputationalRules &rule)
{
    //TODO vylepsit tento hash
    return 1;
}
