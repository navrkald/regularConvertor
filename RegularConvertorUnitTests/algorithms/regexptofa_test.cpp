#include "regexptofa_test.h"

RegExpToFA_test::RegExpToFA_test(QObject *parent) :
    QObject(parent)
{
}

void RegExpToFA_test::initTestCase()
{
    re.init("((ab)*+c)*");
}


void RegExpToFA_test::computeSolution_test()
{
    FiniteAutomata C;
    C.alphabet = QSet<QString>() << "a"<<"b"<<"c";
    C.states = QSet<QString>()<<"0"<<"0\'"<<"0'\'"<<"1"<<"1\'"<<"1'\'"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    C.startState = "6";
    C.finalStates = QSet<QString>()<<"7";

    C.rules.insert(ComputationalRules("0","1","a"));
    C.rules.insert(ComputationalRules("0\'","1\'","b"));
    C.rules.insert(ComputationalRules("1","0\'",EPSILON));
    C.rules.insert(ComputationalRules("1\'","0",EPSILON));
    C.rules.insert(ComputationalRules("2","0",EPSILON));
    C.rules.insert(ComputationalRules("1\'","3",EPSILON));
    C.rules.insert(ComputationalRules("2","3",EPSILON));
    C.rules.insert(ComputationalRules("0\''","1\''","c"));
    C.rules.insert(ComputationalRules("4","2",EPSILON));
    C.rules.insert(ComputationalRules("4","0\''",EPSILON));
    C.rules.insert(ComputationalRules("3","5",EPSILON));
    C.rules.insert(ComputationalRules("1\''","5",EPSILON));
    C.rules.insert(ComputationalRules("6","4",EPSILON));
    C.rules.insert(ComputationalRules("5","7",EPSILON));
    C.rules.insert(ComputationalRules("5","4",EPSILON));
    C.rules.insert(ComputationalRules("6","7",EPSILON));

    RegExpToFA algorithm(re);
    algorithm.computeSolution();
    FiniteAutomata result = algorithm.re.rootNode->correct_FA;

    QCOMPARE(C, result);
}

void RegExpToFA_test::cleanupTestCase()
{
}
