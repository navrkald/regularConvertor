#ifndef DFATOMINFA_TEST_H
#define DFATOMINFA_TEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../RegularConvertor/algorithms/dfatominfa.h"

class DFAtoMinFA_test : public QObject
{
    Q_OBJECT
public:
    explicit DFAtoMinFA_test(QObject *parent = 0);
    FiniteAutomata FA;
    FiniteAutomata MinFA_check;

private slots:
    void initTestCase();
    void computeSolution_test();
    void cleanupTestCase();
};

#endif // DFATOMINFA_TEST_H
