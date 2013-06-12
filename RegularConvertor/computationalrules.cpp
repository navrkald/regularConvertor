#include "computationalrules.h"

ComputationalRules::ComputationalRules(QObject *parent) :
    QObject(parent)
{

}

ComputationalRules::ComputationalRules(QString _from, QString _to, QString _symbol, QObject *parent)
{
    from = _from;
    to = _to;
    symbol= _symbol;
}
