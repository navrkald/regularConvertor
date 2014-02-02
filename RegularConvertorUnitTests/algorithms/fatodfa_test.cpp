#include "fatodfa_test.h"

FaToDFA_test::FaToDFA_test(QObject *parent) :
    QObject(parent)
{
}

void FaToDFA_test::initTestCase()
{
    FA.alphabet << "a" << "b" << "c";
    FA.startState = "s";
    FA.states << "s" << "q1" << "q2" << "f";
    FA.finalStates << "f";
    FA.addRule("s","s","a");
    FA.addRule("f","f","a");
    FA.addRule("s","q1","b");
    FA.addRule("q1","q1","b");
    FA.addRule("s","f","b");
    FA.addRule("q1","f","b");
    FA.addRule("s","f","c");
    FA.addRule("s","q2","c");
    FA.addRule("q2","q2","c");
    FA.addRule("q2","f","c");

    DFA_check.alphabet << "a" << "b" << "c";
    DFA_check.startState = "{s}";
    DFA_check.states << "{s}" << "{f, q1}" << "{f, q2}" <<"{f}";
    DFA_check.finalStates << "{f, q2}" << "{f, q1}" << "{f}";
    DFA_check.addRule("{s}","{s}","a");
    DFA_check.addRule("{s}", "{f, q1}", "b");
    DFA_check.addRule("{s}", "{f, q2}", "c");
    DFA_check.addRule("{f, q1}" ,"{f, q1}", "b");
    DFA_check.addRule("{f, q1}" ,"{f}", "a");
    DFA_check.addRule("{f, q2}" ,"{f, q2}", "c");
    DFA_check.addRule("{f, q2}" ,"{f}", "a");
    DFA_check.addRule("{f}" ,"{f}", "a");


}

void FaToDFA_test::computeSolution_test()
{
    FaToDFA fa_to_dfa_algorithm(FA);
    FiniteAutomata result = fa_to_dfa_algorithm.getDFA();
    QCOMPARE(DFA_check, result);
}

void FaToDFA_test::cleanupTestCase()
{

}
