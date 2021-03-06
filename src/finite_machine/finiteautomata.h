﻿#ifndef FINITEAUTOMATA_H
#define FINITEAUTOMATA_H

#include <QObject>
#include <QtCore>
#include "computationalrules.h"
#include "set/set_of_sets.h"

class FiniteAutomata
{
public:
    //
    //Constructors
    //
    explicit FiniteAutomata();
    FiniteAutomata(QString symbol);
    FiniteAutomata(const FiniteAutomata& _FA);


    void init(QString symbol);
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
    FiniteAutomata removeEpsilon(FiniteAutomata FA);
    void removeEpsilon();
    FiniteAutomata toDFA(FiniteAutomata FA);
    void toDFA();
    FiniteAutomata toMinFA(FiniteAutomata FA);
    void toMinFA();
    FiniteAutomata normalize(FiniteAutomata FA);
    QSet<QString> GetNewInputSymbols(QSet<QString> editedSymbols);
    QSet<QString> GetDeletedInputSymbols(QSet<QString> editedSymbols);
    static FiniteAutomata concatenate(FiniteAutomata FA1, FiniteAutomata FA2);
    static FiniteAutomata iteration(FiniteAutomata FA1);
    static bool AreEquivalent(FiniteAutomata FA1, FiniteAutomata FA2);

    /**
     *  Basic Atomic operations
    **/
    bool addState(QString stateName);
    //Nemuzu odstaranit stav pokud do nej vede sipka
    //Pokud ostranim stav, do ktereho vede sipka muzu ukazat uzivateli varovani
    bool removeState(QString stateName);
    void removeStates(QSet <QString> m_states);
    bool RenameState(QString oldStateName, QString newStateName);
    void SetStartState(QString StateName);
    QString GetStartState();
    void addFinalState(QString StateName);
    void removeFinalState(QString StateName);
    void addSymbol(QString symbol);
    void removeSymbol(QString symbol);
    QList<QString> get_sorted_states();
    QVector<QString> GetSortedAlphabet();
    //TODO parametry
    bool addRule(ComputationalRules newrule);
    bool addRule(QString from, QString to, QString symbol);
    bool removeRule(ComputationalRules rule);
    bool changeSymbolInRule(ComputationalRules rule, QString symbol);
    bool changeRule(ComputationalRules oldrule, ComputationalRules newrule);
    void SetAplhabet(QSet<QString> alphabet);
	void AddAplhabet(QSet<QString> alphabet);
    bool hasEpsilon();
    bool isDeterministic();
    QSet <QString> epsilonCloser(QString state);

    void SetStates(const QSet <QString>& states);
    void SetFinalStates(const QSet <QString>& finalStates);
    QSet <QString> epsilonNeighbours(QString state);
    QSet <ComputationalRules> nonEpsilonRulesOf(QString state);
    QString normalize_chooseSmallestNonprocessed(QList <QString> renamed, QList <QString> processed);
    QString PrintHtmlStates();
	QString PrintHtmlAlphabet();
	QString PrintHtmlRules();
	QString PrintHtmlStartState();
	QString PrintHtmlFinalStates();
	QDataStream& WriteToQDataStream(QDataStream &out) const;
	QDataStream& ReadFromQDataStream(QDataStream &in);

// Getters and setters
public:
    QSet<QString> GetAlphabet() const {return m_alphabet;}

    //
    //Variables
    //
    QSet <QString>  m_states;
    QSet <QString> m_alphabet;
    QSet <ComputationalRules> m_rules;
    QString m_startState;
    QSet <QString> m_finalStates;
    QMap<QString, QPoint> m_coordinates;
    int m_nextId;

private:
    bool canDivide(FiniteAutomata FA,QString symbol, QSet< QSet <QString> > Qm, QSet<QString> X, QSet <QString>& X1, QSet <QString>& X2);
};

FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2);
bool operator==(const FiniteAutomata FA1, const FiniteAutomata FA2);
QDataStream& operator<<(QDataStream& out, const FiniteAutomata& fa);
QDataStream& operator>>(QDataStream& in, FiniteAutomata& fa);

/**
  * Overloading operator for debugging purposes
  */
QDebug operator<< (QDebug d, const FiniteAutomata& FA);

#endif // FINITEAUTOMATA_H
