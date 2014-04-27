#include "computationalrules.h"

ComputationalRules::ComputationalRules()
{
    from = "";
    to = "";
    symbol= "";
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
        //qDebug() << "From= " <<from << "\nSymbol= " << symbol << "\nto= " <<to;
    }
}

ComputationalRules::ComputationalRules(const ComputationalRules& _object)
{
    this->from = _object.from;
    this->to = _object.to;
    this->symbol = _object.symbol;
}

ComputationalRules &ComputationalRules::operator=(const ComputationalRules& _rule)
{
    this->from = _rule.from;
    this->to = _rule.to;
    this->symbol = _rule.symbol;
    return *this;
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

uint qHash(const ComputationalRules&)
{
    //TODO vylepsit tento hash
    return 1;
}


QSet<QString> getFroms(QList<ComputationalRules> list)
{
    QSet<QString> froms;
    foreach(ComputationalRules rule, list)
    {
        froms.insert(rule.from);
    }
    return froms;
}


QSet<QString> getTos(QList<ComputationalRules> list)
{
    QSet<QString> tos;
    foreach(ComputationalRules rule, list)
    {
        tos.insert(rule.to);
    }
    return tos;
}

QDataStream &operator<<(QDataStream& out, const ComputationalRules& rule)
{
    out << rule.from << rule.symbol << rule.to;
    return out;
}

QDataStream &operator>>(QDataStream &in, ComputationalRules &rule)
{
    in >> rule.from >> rule.symbol >> rule.to;
    return in;
}
