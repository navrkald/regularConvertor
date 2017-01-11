#ifndef REMOVEEPSILON_TEST_H
#define REMOVEEPSILON_TEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../RegularConvertor/algorithms/removeepsilon.h"

class RemoveEpsilon_test : public QObject
{
    Q_OBJECT
public:
    explicit RemoveEpsilon_test(QObject *parent = 0);

private:
    FiniteAutomata remove_epsilon_FA1;
    FiniteAutomata remove_epsilon_FA1_check;

private slots:
    void initTestCase();
    void removeEpsilon_test();
    void cleanupTestCase();
    
};

#endif // REMOVEEPSILON_TEST_H
