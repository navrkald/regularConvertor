#include "dfatominfa_test.h"

DFAtoMinFA_test::DFAtoMinFA_test(QObject *parent) :
    QObject(parent)
{
}

void DFAtoMinFA_test::initTestCase()
{
    toMinFA_FA1.alphabet << "a" << "b";
    toMinFA_FA1.states << "s" << "q1" << "q2" << "q3" << "q4" << "f";
    toMinFA_FA1.startState = "s";
    toMinFA_FA1.finalStates << "f" << "s";
    toMinFA_FA1.addRule("q1","q1","a");
    toMinFA_FA1.addRule("q2","q2","a");
    toMinFA_FA1.addRule("s","f","a");
    toMinFA_FA1.addRule("f","s","a");
    toMinFA_FA1.addRule("q3","q4","a");
    toMinFA_FA1.addRule("q4","q3","a");
    toMinFA_FA1.addRule("q1","s","b");
    toMinFA_FA1.addRule("q3","q1","b");
    toMinFA_FA1.addRule("s","q3","b");
    toMinFA_FA1.addRule("f","q4","b");
    toMinFA_FA1.addRule("q2","f","b");
    toMinFA_FA1.addRule("q4","q2","b");

    toMinFA_FA1_check.alphabet << "a" << "b";
    toMinFA_FA1_check.states << "{{f}, {s}}" << "{{q1}, {q2}}" << "{{q3}, {q4}}";
    toMinFA_FA1_check.startState = "{{f}, {s}}";
    toMinFA_FA1_check.finalStates << "{{f}, {s}}";
    toMinFA_FA1_check.addRule("{{f}, {s}}", "{{f}, {s}}", "a");
    toMinFA_FA1_check.addRule("{{q1}, {q2}}", "{{q1}, {q2}}", "a");
    toMinFA_FA1_check.addRule("{{q3}, {q4}}", "{{q3}, {q4}}", "a");
    toMinFA_FA1_check.addRule("{{q1}, {q2}}", "{{f}, {s}}", "b");
    toMinFA_FA1_check.addRule("{{f}, {s}}", "{{q3}, {q4}}", "b");
    toMinFA_FA1_check.addRule("{{q3}, {q4}}", "{{q1}, {q2}}", "b");
}

void DFAtoMinFA_test::computeSolution_test()
{
    DFAtoMinFA minFA_algorithm(toMinFA_FA1);
    FiniteAutomata result = minFA_algorithm.computeSolution();
    QCOMPARE (result, toMinFA_FA1_check);
}

void DFAtoMinFA_test::cleanupTestCase()
{

}
