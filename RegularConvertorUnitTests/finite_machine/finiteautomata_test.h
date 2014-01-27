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
    void cleanupTestCase();
};

#endif // FINITEAUTOMATA_TEST_H
