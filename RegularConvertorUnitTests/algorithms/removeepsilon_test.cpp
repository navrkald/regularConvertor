#include "removeepsilon_test.h"

RemoveEpsilon_test::RemoveEpsilon_test(QObject *parent) :
    QObject(parent)
{
}

void RemoveEpsilon_test::initTestCase()
{
    FA1.states << "s" << "q1" << "q2" << "f";
    FA1.alphabet << "a" << "b" << "c";
    FA1.startState = "s";
    FA1.finalStates << "s" << "q1" << "q2" << "f";
    FA1.addRule("s","s","a");
    FA1.addRule("s","q1",EPSILON);
    FA1.addRule("q1","q1","b");
    FA1.addRule("q1","f","b");
    FA1.addRule("s","q2",EPSILON);
    FA1.addRule("q2","q2","c");
    FA1.addRule("q2","f","c");
    FA1.addRule("f","f","a");


    //
    FA1_check.states << "s" << "q1" << "q2" << "f";
    FA1_check.alphabet << "a" << "b" << "c";
    FA1_check.startState = "s";
    FA1_check.finalStates << "s" << "q1" << "q2" << "f";
    FA1_check.addRule("s","s","a");
    FA1_check.addRule("q1","q1","b");
    FA1_check.addRule("f","f","a");
    FA1_check.addRule("q2","q2","c");
    FA1_check.addRule("s","q1","b");
    FA1_check.addRule("q1","f","b");
    FA1_check.addRule("s","f","b");
    FA1_check.addRule("s","f","c");
    FA1_check.addRule("s","q2","c");
    FA1_check.addRule("q2","f","c");
}

void RemoveEpsilon_test::removeEpsilon_test()
{
    //TODO vymyslet priklad kde nebudou na konci po prevodu stejne koncove stavy
    RemoveEpsilon rem(FA1);
    FiniteAutomata result = rem.removeEpsilon();
    QCOMPARE(FA1_check, result);
}

void RemoveEpsilon_test::cleanupTestCase()
{
}
