#ifndef COMPUTATIONALRULES_H
#define COMPUTATIONALRULES_H

//#include <QObject>
#include <QRegExp>
#include <QDebug>
#define EPSILON "ε"
#define EMPTYSET "Ø"

class ComputationalRules// : public QObject
{
//    Q_OBJECT
public:
    explicit ComputationalRules();
    ComputationalRules(QString _from, QString _to, QString _symbol);
    ComputationalRules(QString rule);
    ComputationalRules(const ComputationalRules& _object);//Toto je kopirovaci konstruktor
    ComputationalRules& operator=(const ComputationalRules& _rule);
    QString toString() const{return from + " " + symbol + "-> " + to;}
    void Init(QString from, QString to, QString symbol)
    {
        this->from = from;
        this->to = to;
        this->symbol = symbol;
    }

    QString getFrom(){return from;}
    QString getTo(){return from;}
    QString getSymbol(){return from;}
    void setFrom(QString f){from = f;}
    void setTo(QString t){to = t;}
    void setSymbol(QString s){symbol = s;}
    bool operator==(const ComputationalRules& secondRule) const
        {
            return ( (this->from == secondRule.from) &&
                    (this->symbol==secondRule.symbol) &&
                     this->to == secondRule.to);
        }
    const ComputationalRules operator=(const ComputationalRules& rule) const
    {
         ComputationalRules returnRule(rule.from,rule.to,rule.symbol);
        return returnRule;
    }

public:
    QString from;
    QString to;
    QString symbol;
};

bool operator<(const ComputationalRules& r1, const ComputationalRules& r2);

QDebug operator<<(QDebug dbg, const ComputationalRules& rule);
QDataStream &operator<<(QDataStream& out, const ComputationalRules& rule);
QDataStream &operator>>(QDataStream &in, ComputationalRules &rule);

bool lessThan(const ComputationalRules& r1, const ComputationalRules& r2);
QSet <QString> getFroms(QList <ComputationalRules> list);
QSet <QString> getTos(QList <ComputationalRules> list);
uint qHash(const ComputationalRules& rule);
#endif // COMPUTATIONALRULES_H
