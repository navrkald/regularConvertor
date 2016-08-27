#ifndef PDACOMPOTUTATIONALRULES_H
#define PDACOMPOTUTATIONALRULES_H

#include "computationalrules.h"

class CPDACompotutationalRule : public ComputationalRules
{
public:
    explicit CPDACompotutationalRule() : ComputationalRules(), m_popSymol("") {}
    CPDACompotutationalRule(
            QString from,
            QString to,
            QString symbol,
            QString popSymol,
            QVector<QString> pushSymbols)
        : ComputationalRules(from, to, symbol), m_popSymol(popSymol), m_pushSymbols(pushSymbols) {}
    CPDACompotutationalRule(
            QString from,
            QString to,
            QString symbol,
            QString popSymol,
            QString pushSymbol)
        : ComputationalRules(from, to, symbol), m_popSymol(popSymol)
    {
        QVector<QString> pushSymbols; pushSymbols.append(pushSymbol); m_pushSymbols = pushSymbols;
    }
    CPDACompotutationalRule(QString rule);

    CPDACompotutationalRule(const CPDACompotutationalRule& r) : ComputationalRules(r.from, r.to, r.symbol)
    {
        this->m_popSymol = r.m_popSymol;
        this->m_pushSymbols = r.m_pushSymbols;
    }

    void Init(QString from, QString to, QVector<QString> pushSymbols, QString popSymol, QString symbol)
    {
        ComputationalRules::Init(from, to, symbol);
        this->m_pushSymbols = pushSymbols;
        this->m_popSymol = popSymol;
    }

    CPDACompotutationalRule& operator=(const CPDACompotutationalRule& r)
    {
        CPDACompotutationalRule(r.from, r.to, r.symbol, r.m_popSymol, r.m_pushSymbols);
//        Init(r.from, r.to, r.pushSymbol, r.popSymol, r.symbol);
        return *this;
    }

    QString toString() const{return from + " " + m_popSymol + " " + symbol + "-> " + m_pushSymbols.toList().join(", ") + " " + to;}

		static QString ToArrowText(const QSet<CPDACompotutationalRule>& pdaRules);

    bool operator==(const CPDACompotutationalRule& secondRule) const
        {
            return ( ComputationalRules::operator ==(secondRule) &&
                     this->m_pushSymbols == secondRule.m_pushSymbols &&
                     this->m_popSymol == secondRule.m_popSymol
                     );
        }
    const CPDACompotutationalRule operator=(const CPDACompotutationalRule& rule) const
    {
        CPDACompotutationalRule returnRule(rule.from, rule.to, rule.symbol, rule.m_popSymol, rule.m_pushSymbols);
        return returnRule;
    }

public:
    QString GetPopSymbol() { return m_popSymol; }
		QVector<QString> GetPushSymbols() { return m_pushSymbols; }
		QSet<QString> GetStackSymbols();

    QString m_popSymol;
    QVector<QString> m_pushSymbols;
};

#endif // PDACOMPOTUTATIONALRULES_H
