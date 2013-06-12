#ifndef FINITEAUTOMATA_H
#define FINITEAUTOMATA_H

#include <QObject>
#include <QtCore>
#include <computationalrules.h>

class FiniteAutomata : public QObject
{
    Q_OBJECT
public:
    explicit FiniteAutomata(QObject *parent = 0);

private:
    QSet <QString>  nodes;
    QSet <QString> alphabet;
    QSet <ComputationalRules> rules;
    QString starNode;
    QSet <QString> finalStates;
    
signals:
    
public slots:
    
};

#endif // FINITEAUTOMATA_H
