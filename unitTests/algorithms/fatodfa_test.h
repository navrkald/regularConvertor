#ifndef FATODFA_TEST_H
#define FATODFA_TEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../RegularConvertor/algorithms/fatodfa.h"

class FaToDFA_test : public QObject
{
    Q_OBJECT
public:
    explicit FaToDFA_test(QObject *parent = 0);
    FiniteAutomata toDFA_FA1;
    FiniteAutomata toDFA_FA1_check;

private slots:
    void initTestCase();
    void computeSolution_test();
    void cleanupTestCase();
};

#endif // FATODFA_TEST_H
