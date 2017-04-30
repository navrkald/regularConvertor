#include "computationalrules.h"

ComputationalRules::ComputationalRules()
{
    Init("","","");
}

ComputationalRules::ComputationalRules(QString from, QString to, QString symbol)
{
    Init(from, to, symbol);
}

ComputationalRules::ComputationalRules(QString rule)
{
    const QRegExp ruleRegExp("^(\\w+)\\s+(\\w)\\s*->\\s*(\\w+)$");
    if(ruleRegExp.exactMatch(rule))
    {
        ruleRegExp.indexIn(rule);
        from = ruleRegExp.cap(1);
        symbol = ruleRegExp.cap(2);
        to = ruleRegExp.cap(3);
    }
}

ComputationalRules::ComputationalRules(const ComputationalRules& r)
{
    Init(r.from, r.to, r.symbol);
}

ComputationalRules &ComputationalRules::operator=(const ComputationalRules& r)
{
    Init(r.from, r.to, r.symbol);
    return *this;
}

ComputationalRules::operator QString()
{
	return toString();
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

QSet<QString> ComputationalRulesToQSetOfStrings(QSet<ComputationalRules> rules)
{
	QSet<QString> out;
	foreach(ComputationalRules rule, rules) {
		out.insert(rule);
	}
	return out;
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
