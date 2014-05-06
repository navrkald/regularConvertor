#include "fatodfa_test.h"

FaToDFA_test::FaToDFA_test(QObject *parent) :
    QObject(parent)
{
}

void FaToDFA_test::initTestCase()
{
    toDFA_FA1.alphabet << "a" << "b" << "c";
    toDFA_FA1.startState = "s";
    toDFA_FA1.states << "s" << "q1" << "q2" << "f";
    toDFA_FA1.finalStates << "f";
    toDFA_FA1.addRule("s","s","a");
    toDFA_FA1.addRule("f","f","a");
    toDFA_FA1.addRule("s","q1","b");
    toDFA_FA1.addRule("q1","q1","b");
    toDFA_FA1.addRule("s","f","b");
    toDFA_FA1.addRule("q1","f","b");
    toDFA_FA1.addRule("s","f","c");
    toDFA_FA1.addRule("s","q2","c");
    toDFA_FA1.addRule("q2","q2","c");
    toDFA_FA1.addRule("q2","f","c");

    toDFA_FA1_check.alphabet << "a" << "b" << "c";
    toDFA_FA1_check.startState = "{s}";
    toDFA_FA1_check.states << "{s}" << "{f, q1}" << "{f, q2}" <<"{f}";
    toDFA_FA1_check.finalStates << "{f, q2}" << "{f, q1}" << "{f}";
    toDFA_FA1_check.addRule("{s}","{s}","a");
    toDFA_FA1_check.addRule("{s}", "{f, q1}", "b");
    toDFA_FA1_check.addRule("{s}", "{f, q2}", "c");
    toDFA_FA1_check.addRule("{f, q1}" ,"{f, q1}", "b");
    toDFA_FA1_check.addRule("{f, q1}" ,"{f}", "a");
    toDFA_FA1_check.addRule("{f, q2}" ,"{f, q2}", "c");
    toDFA_FA1_check.addRule("{f, q2}" ,"{f}", "a");
    toDFA_FA1_check.addRule("{f}" ,"{f}", "a");
}

void FaToDFA_test::computeSolution_test()
{
    FaToDFA fa_to_dfa_algorithm(toDFA_FA1);
    FiniteAutomata result = fa_to_dfa_algorithm.computeSolution();
    QCOMPARE(toDFA_FA1_check, result);
}

void FaToDFA_test::cleanupTestCase()
{

}
