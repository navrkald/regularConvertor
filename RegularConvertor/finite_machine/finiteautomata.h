#ifndef FINITEAUTOMATA_H
#define FINITEAUTOMATA_H

#include <QObject>
#include <QtCore>
#include "computationalrules.h"

class FiniteAutomata //: public QObject
{
//    Q_OBJECT
public:
    explicit FiniteAutomata();
    FiniteAutomata(QString symbol);
    void init(QString symbol);
    int nextId;
    bool isStateUnique(QString state);
    QString createUniqueName();
    QList <ComputationalRules> findRule_Symbol(QString symbol);

    QList <ComputationalRules> findRule_FromSymbol(QString from, QString symbol);
    QList <ComputationalRules> findRule_FromSymbol(QSet <QString> from, QString symbol);
    QList <ComputationalRules> findRule_From(QString from);
    QList <QString> findState_from(QString from);
    QList <QString> findState_to(QString to);
    QList <QString> getReachableStates();
    QList <QString> getReverseReachableStates();
    void removeUnreachableStates();
    FiniteAutomata removeUnreachableStates(FiniteAutomata FA);
    FiniteAutomata removeNonTerminatingStates(FiniteAutomata FA);
    void removeNonTerminatingStates();
    FiniteAutomata makeWellDefined(FiniteAutomata FA);
    void makeWellDefined();


public:
    QSet <QString>  states;
    QSet <QString> alphabet;
    QSet <ComputationalRules> rules;
    QString startState;
    QSet <QString> finalStates;

    
    static FiniteAutomata concatenate(FiniteAutomata FA1, FiniteAutomata FA2);
    static FiniteAutomata iteration(FiniteAutomata FA1);

//signals:
    
//public slots:
    bool addState(QString stateName);
    //Nemuzu odstaranit stav pokud do nej vede sipka
    //Pokud ostranim stav, do ktereho vede sipka muzu ukazat uzivateli varovani
    bool removeState(QString stateName);
    void removeStates(QSet <QString> states);
    bool renameState(QString oldStateName, QString newStateName);
    void changeStartState(QString StateName);
    void addFinalState(QString StateName);
    void removeFinalState(QString StateName);
    void addSymbol(QString symbol);
    void removeSymbol(QString symbol);
    //TODO parametry
    bool addRule(ComputationalRules newrule);
    bool addRule(QString from, QString to, QString symbol);
    void removeRule(ComputationalRules rule);
    bool changeSymbolInRule(ComputationalRules rule, QString symbol);
    bool changeRule(ComputationalRules oldrule, ComputationalRules newrule);
    QSet <QString> epsilonCloser(QString state);
    QSet <QString> epsilonNeighbours(QString state);
    QSet <ComputationalRules> nonEpsilonRulesOf(QString state);


};

FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2);
bool operator==(const FiniteAutomata FA1, const FiniteAutomata FA2);

#endif // FINITEAUTOMATA_H
