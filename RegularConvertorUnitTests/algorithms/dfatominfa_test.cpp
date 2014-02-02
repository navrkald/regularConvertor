#include "dfatominfa_test.h"

DFAtoMinFA_test::DFAtoMinFA_test(QObject *parent) :
    QObject(parent)
{
}

void DFAtoMinFA_test::initTestCase()
{
    FA.alphabet << "a" << "b";
    FA.states << "s" << "q1" << "q2" << "q3" << "q4" << "f";
    FA.startState = "s";
    FA.finalStates << "f" << "s";
    FA.addRule("q1","q1","a");
    FA.addRule("q2","q2","a");
    FA.addRule("s","f","a");
    FA.addRule("f","s","a");
    FA.addRule("q3","q4","a");
    FA.addRule("q4","q3","a");
    FA.addRule("q1","s","b");
    FA.addRule("q3","q1","b");
    FA.addRule("s","q3","b");
    FA.addRule("f","q4","b");
    FA.addRule("q2","f","b");
    FA.addRule("q4","q2","b");

    MinFA_check.alphabet << "a" << "b";
    MinFA_check.states << "{f, s}" << "{q1, q2}" << "{q3, q4}";
    MinFA_check.startState = "{f, s}";
    MinFA_check.finalStates << "{f, s}";
    MinFA_check.addRule("{f, s}", "{f, s}", "a");
    MinFA_check.addRule("{q1, q2}", "{q1, q2}", "a");
    MinFA_check.addRule("{q3, q4}", "{q3, q4}", "a");
    MinFA_check.addRule("{q1, q2}", "{f, s}", "b");
    MinFA_check.addRule("{f, s}", "{q3, q4}", "b");
    MinFA_check.addRule("{q3, q4}", "{q1, q2}", "b");
}

void DFAtoMinFA_test::computeSolution_test()
{
    DFAtoMinFA minFA_algorithm(FA);
    FiniteAutomata result = minFA_algorithm.convert();
    QCOMPARE (result, MinFA_check);
}

void DFAtoMinFA_test::cleanupTestCase()
{

}
