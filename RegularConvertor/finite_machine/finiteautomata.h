    #ifndef FINITEAUTOMATA_H
#define FINITEAUTOMATA_H

#include <QObject>
#include <QtCore>
#include "computationalrules.h"
#include "set/set_of_sets.h"

class FiniteAutomata //: public QObject
{
//    Q_OBJECT
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

    //
    //Variables
    //
    QSet <QString>  states;
    QSet <QString> alphabet;
    QSet <ComputationalRules> rules;
    QString startState;
    QSet <QString> finalStates;
    QMap<QString, QPoint> coordinates;
    int nextId;

    
    static FiniteAutomata concatenate(FiniteAutomata FA1, FiniteAutomata FA2);
    static FiniteAutomata iteration(FiniteAutomata FA1);
    static bool areEquivalent(FiniteAutomata FA1, FiniteAutomata FA2);

    /**
     *  Basic Atomic operations
    **/
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
    QList<QString> get_sorted_states();
    //TODO parametry
    bool addRule(ComputationalRules newrule);
    bool addRule(QString from, QString to, QString symbol);
    void removeRule(ComputationalRules rule);
    bool changeSymbolInRule(ComputationalRules rule, QString symbol);
    bool changeRule(ComputationalRules oldrule, ComputationalRules newrule);

    bool hasEpsilon();
    bool isDeterministic();
    QSet <QString> epsilonCloser(QString state);

    QSet <QString> epsilonNeighbours(QString state);
    QSet <ComputationalRules> nonEpsilonRulesOf(QString state);
    QString normalize_chooseSmallestNonprocessed(QList <QString> renamed, QList <QString> processed);


private:
    bool canDivide(FiniteAutomata FA,QString symbol, QSet< QSet <QString> > Qm, QSet<QString> X, QSet <QString> &X1, QSet <QString> &X2);


};

FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2);
bool operator==(const FiniteAutomata FA1, const FiniteAutomata FA2);

/**
  * Overloading operator for debugging purposes
  */
QDebug operator<< (QDebug d, const FiniteAutomata &FA);

QDataStream &operator<<(QDataStream &out, const FiniteAutomata &FA);
QDataStream &operator>>(QDataStream &in, FiniteAutomata &FA);
#endif // FINITEAUTOMATA_H
