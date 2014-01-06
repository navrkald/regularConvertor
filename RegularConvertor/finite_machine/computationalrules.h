#ifndef COMPUTATIONALRULES_H
#define COMPUTATIONALRULES_H

//#include <QObject>
#include <QRegExp>
#include <QDebug>

#define EPSILON "ε"

class ComputationalRules// : public QObject
{
//    Q_OBJECT
public:
    explicit ComputationalRules();
    ComputationalRules(QString _from, QString _to, QString _symbol);
    ComputationalRules(QString rule);
    ComputationalRules(const ComputationalRules& object);//Toto je kopirovaci konstruktor
    QString toString() {return from + " " + symbol + "-> " + to;}

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
    QString from;
    QString to;
    QString symbol;
private:

//signals:
    
//public slots:
    
};
uint qHash(const ComputationalRules& rule);
#endif // COMPUTATIONALRULES_H
