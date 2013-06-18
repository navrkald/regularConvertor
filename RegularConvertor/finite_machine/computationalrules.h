#ifndef COMPUTATIONALRULES_H
#define COMPUTATIONALRULES_H

#include <QObject>

class ComputationalRules : public QObject
{
    Q_OBJECT
public:
    explicit ComputationalRules(QObject *parent = 0);
    ComputationalRules(QString _from, QString _to, QString _symbol, QObject *parent = 0);
    QString toString() {return from + " " + symbol + "-> " + to;}
    QString getFrom(){return from;}
    QString getTo(){return from;}
    QString getSymbol(){return from;}
    void setFrom(QString f){from = f;}
    void setTo(QString t){to = t;}
    void setSymbol(QString s){symbol = s;}
private:
    QString from;
    QString to;
    QString symbol;
signals:
    
public slots:
    
};

#endif // COMPUTATIONALRULES_H
