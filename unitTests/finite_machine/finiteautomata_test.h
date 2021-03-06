#ifndef FINITEAUTOMATA_TEST_H
#define FINITEAUTOMATA_TEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../RegularConvertor/finite_machine/finiteautomata.h"
#include "../../RegularConvertor/finite_machine/computationalrules.h"

class FiniteAutomata_test : public QObject
{
    Q_OBJECT
public:
    explicit FiniteAutomata_test(QObject *parent = 0);
private:
    FiniteAutomata A;
    FiniteAutomata B;
    FiniteAutomata simpleFA;
    FiniteAutomata epsilon_FA;

    FiniteAutomata remove_epsilon_FA1;
    FiniteAutomata remove_epsilon_FA1_check;

    FiniteAutomata toDFA_FA1;
    FiniteAutomata toDFA_FA1_check;

    FiniteAutomata unreachabe_states_FA;
    FiniteAutomata unreachabe_states_check_FA;

    FiniteAutomata nonterminating_states_FA;
    FiniteAutomata nonterminating_states_check_FA;

    FiniteAutomata not_makeWellDefined_FA;
    FiniteAutomata makeWellDefined_check_FA;

    FiniteAutomata toMinFA_FA1;
    FiniteAutomata toMinFA_FA1_check;

    FiniteAutomata normalize_FA1;
    FiniteAutomata normalize_FA1_check;

    //Check variables
    FiniteAutomata concatenate_check;
    FiniteAutomata alternate_check;
    FiniteAutomata simple_iterate_check;
    FiniteAutomata iterate_check;
    QSet <QString> epsilonCloser_6_check;
    QSet <QString> epsilonCloser_2_check;
    QSet <QString> epsilonCloser_3_check;


private slots:
    void initTestCase();
    void concatenate_test();
    void alternate_test();
    void simple_iterate_test();
    void iterate_test();
    void epsilonCloser_test();
    void remove_epsilon_FA1_test();
    void toDFA_test();
    void removeUnreachableStates_test();
    void removeNonTerminatingStates_test();
    void makeWellDefined_test();
    void toMinFA_test();
    void normalize();
    void cleanupTestCase();
};

#endif // FINITEAUTOMATA_TEST_H
