#include "removeepsilon_test.h"

RemoveEpsilon_test::RemoveEpsilon_test(QObject *parent) :
    QObject(parent)
{
}

void RemoveEpsilon_test::initTestCase()
{
    remove_epsilon_FA1.states << "s" << "q1" << "q2" << "f";
    remove_epsilon_FA1.alphabet << "a" << "b" << "c";
    remove_epsilon_FA1.startState = "s";
    remove_epsilon_FA1.finalStates << "s" << "q1" << "q2" << "f";
    remove_epsilon_FA1.addRule("s","s","a");
    remove_epsilon_FA1.addRule("s","q1",EPSILON);
    remove_epsilon_FA1.addRule("q1","q1","b");
    remove_epsilon_FA1.addRule("q1","f","b");
    remove_epsilon_FA1.addRule("s","q2",EPSILON);
    remove_epsilon_FA1.addRule("q2","q2","c");
    remove_epsilon_FA1.addRule("q2","f","c");
    remove_epsilon_FA1.addRule("f","f","a");


    //
    remove_epsilon_FA1_check.states << "s" << "q1" << "q2" << "f";
    remove_epsilon_FA1_check.alphabet << "a" << "b" << "c";
    remove_epsilon_FA1_check.startState = "s";
    remove_epsilon_FA1_check.finalStates << "s" << "q1" << "q2" << "f";
    remove_epsilon_FA1_check.addRule("s","s","a");
    remove_epsilon_FA1_check.addRule("q1","q1","b");
    remove_epsilon_FA1_check.addRule("f","f","a");
    remove_epsilon_FA1_check.addRule("q2","q2","c");
    remove_epsilon_FA1_check.addRule("s","q1","b");
    remove_epsilon_FA1_check.addRule("q1","f","b");
    remove_epsilon_FA1_check.addRule("s","f","b");
    remove_epsilon_FA1_check.addRule("s","f","c");
    remove_epsilon_FA1_check.addRule("s","q2","c");
    remove_epsilon_FA1_check.addRule("q2","f","c");
}

void RemoveEpsilon_test::removeEpsilon_test()
{
    //TODO vymyslet priklad kde nebudou na konci po prevodu stejne koncove stavy
    RemoveEpsilon rem(remove_epsilon_FA1);
    FiniteAutomata result = rem.computeSolution();
    QCOMPARE(remove_epsilon_FA1_check, result);
}

void RemoveEpsilon_test::cleanupTestCase()
{
}
