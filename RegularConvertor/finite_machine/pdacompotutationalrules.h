#ifndef PDACOMPOTUTATIONALRULES_H
#define PDACOMPOTUTATIONALRULES_H

#include "computationalrules.h"

class CPDACompotutationalRule : public ComputationalRules
{
public:
    explicit CPDACompotutationalRule() : ComputationalRules(), popSymol(""), pushSymbol("") {}
    CPDACompotutationalRule(
            QString from,
            QString to,
            QString symbol,
            QString popSymol,
            QString pushSymbol)
        : ComputationalRules(from, to, symbol), popSymol(popSymol), pushSymbol(pushSymbol) {}
    CPDACompotutationalRule(QString rule);
    CPDACompotutationalRule(const CPDACompotutationalRule& r) : ComputationalRules(r.from, r.to, r.symbol)
    {
        this->popSymol = r.popSymol;
        this->pushSymbol = r.pushSymbol;
    }

    void Init(QString from, QString to, QString pushSymbol, QString popSymol, QString symbol)
    {
        ComputationalRules::Init(from, to, symbol);
        this->pushSymbol = pushSymbol;
        this->popSymol = popSymol;
    }

    CPDACompotutationalRule& operator=(const CPDACompotutationalRule& r)
    {
        CPDACompotutationalRule(r.from, r.to, r.symbol, r.popSymol, r.pushSymbol);
//        Init(r.from, r.to, r.pushSymbol, r.popSymol, r.symbol);
        return *this;
    }

    QString toString() const{return from + " " + popSymol + " " + symbol + "-> " + pushSymbol + " " + to;}

    bool operator==(const CPDACompotutationalRule& secondRule) const
        {
            return ( ComputationalRules::operator ==(secondRule) &&
                     this->pushSymbol == secondRule.pushSymbol &&
                     this->popSymol == secondRule.popSymol
                     );
        }
    const CPDACompotutationalRule operator=(const CPDACompotutationalRule& rule) const
    {
         CPDACompotutationalRule returnRule(rule.from,rule.popSymol, rule.symbol, rule.pushSymbol, rule.to);
        return returnRule;
    }

public:
    QString popSymol;
    QString pushSymbol;
};

#endif // PDACOMPOTUTATIONALRULES_H
