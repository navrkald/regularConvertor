#include "finiteautomata_test.h"

FiniteAutomata_test::FiniteAutomata_test(QObject *parent): QObject(parent)
{
}

void FiniteAutomata_test::initTestCase()
{
    A.alphabet.insert("a");
    A.alphabet.insert("b");
    A.alphabet.insert("c");
    A.alphabet.insert("d");
    A.states.insert("1");
    A.states.insert("2");
    A.states.insert("3");
    A.states.insert("4");
    A.states.insert("5");
    A.startState = "1";
    A.finalStates.insert("3");
    A.finalStates.insert("4");
    A.finalStates.insert("5");
    A.rules.insert(ComputationalRules("1","2","a"));
    A.rules.insert(ComputationalRules("2","4","b"));
    A.rules.insert(ComputationalRules("4","1","a"));
    A.rules.insert(ComputationalRules("3","5","a"));
    A.rules.insert(ComputationalRules("4","4","c"));
    A.rules.insert(ComputationalRules("5","3","d"));


    B.alphabet.insert("c");
    B.alphabet.insert("d");
    B.alphabet.insert("e");
    B.alphabet.insert("f");
    B.states.insert("4");
    B.states.insert("5");
    B.states.insert("6");
    B.states.insert("7");
    B.states.insert("8");
    B.states.insert("9");
    B.states.insert("10");
    B.startState="4";
    B.finalStates.insert("8");
    B.finalStates.insert("9");
    B.finalStates.insert("10");
    B.rules.insert(ComputationalRules("4","6","c"));
    B.rules.insert(ComputationalRules("6","5","d"));
    B.rules.insert(ComputationalRules("5","9","e"));
    B.rules.insert(ComputationalRules("4","4","e"));
    B.rules.insert(ComputationalRules("9","7","f"));
    B.rules.insert(ComputationalRules("7","10","c"));
    B.rules.insert(ComputationalRules("10","4","d"));
    B.rules.insert(ComputationalRules("10","8","d"));
    B.rules.insert(ComputationalRules("8","8","e"));
    B.rules.insert(ComputationalRules("8","4","f"));

    simpleFA.alphabet.insert("a");
    simpleFA.states.insert("1");
    simpleFA.states.insert("2");
    simpleFA.startState = "1";
    simpleFA.finalStates.insert("2");
    simpleFA.rules.insert(ComputationalRules("1","2","a"));

}

void FiniteAutomata_test::concatenate_test()
{
    FiniteAutomata C;
    C.alphabet = QSet<QString>() << "a"<<"b"<<"c"<<"d"<<"e"<<"f";
    C.states = QSet<QString>()<<"1"<<"2"<<"3"<<"4"<<"4'"<<"5"<<"5'"<<"6"<<"7"<<"8"<<"9"<<"10";
    C.startState = "1";
    C.finalStates = QSet<QString>()<<"8"<<"9"<<"10";

    //A rules
    C.rules.insert(ComputationalRules("1","2","a"));
    C.rules.insert(ComputationalRules("2","4","b"));
    C.rules.insert(ComputationalRules("4","1","a"));
    C.rules.insert(ComputationalRules("3","5","a"));
    C.rules.insert(ComputationalRules("4","4","c"));
    C.rules.insert(ComputationalRules("5","3","d"));

    //epsilon rules
    C.rules.insert(ComputationalRules("3","4'",EPSILON));
    C.rules.insert(ComputationalRules("4","4'",EPSILON));
    C.rules.insert(ComputationalRules("5","4'",EPSILON));

    //B rules
    C.rules.insert(ComputationalRules("4'","6","c"));
    C.rules.insert(ComputationalRules("6","5'","d"));
    C.rules.insert(ComputationalRules("5'","9","e"));
    C.rules.insert(ComputationalRules("4'","4'","e"));
    C.rules.insert(ComputationalRules("9","7","f"));
    C.rules.insert(ComputationalRules("7","10","c"));
    C.rules.insert(ComputationalRules("10","4'","d"));
    C.rules.insert(ComputationalRules("10","8","d"));
    C.rules.insert(ComputationalRules("8","8","e"));
    C.rules.insert(ComputationalRules("8","4'","f"));

    FiniteAutomata result = FiniteAutomata::concatenate(A,B);

    QCOMPARE(result, C);
}

void FiniteAutomata_test::alternate_test()
{
    FiniteAutomata C;
    C.alphabet = QSet<QString>() << "a"<<"b"<<"c"<<"d"<<"e"<<"f";
    C.states = QSet<QString>()<<"0"<<"1"<<"2"<<"3"<<"4"<<"4'"<<"5"<<"5'"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11";
    C.startState = "0";
    C.finalStates = QSet<QString>()<<"11";

    //A rules
    C.rules.insert(ComputationalRules("1","2","a"));
    C.rules.insert(ComputationalRules("2","4","b"));
    C.rules.insert(ComputationalRules("4","1","a"));
    C.rules.insert(ComputationalRules("3","5","a"));
    C.rules.insert(ComputationalRules("4","4","c"));
    C.rules.insert(ComputationalRules("5","3","d"));

    //epsilon rules
    //new start to old starts
    C.rules.insert(ComputationalRules("0","1",EPSILON));
    C.rules.insert(ComputationalRules("0","4'",EPSILON));
    //old ends o new end
    C.rules.insert(ComputationalRules("8","11",EPSILON));
    C.rules.insert(ComputationalRules("9","11",EPSILON));
    C.rules.insert(ComputationalRules("10","11",EPSILON));
    C.rules.insert(ComputationalRules("3","11",EPSILON));
    C.rules.insert(ComputationalRules("4","11",EPSILON));
    C.rules.insert(ComputationalRules("5","11",EPSILON));

    //B rules
    C.rules.insert(ComputationalRules("4'","6","c"));
    C.rules.insert(ComputationalRules("6","5'","d"));
    C.rules.insert(ComputationalRules("5'","9","e"));
    C.rules.insert(ComputationalRules("4'","4'","e"));
    C.rules.insert(ComputationalRules("9","7","f"));
    C.rules.insert(ComputationalRules("7","10","c"));
    C.rules.insert(ComputationalRules("10","4'","d"));
    C.rules.insert(ComputationalRules("10","8","d"));
    C.rules.insert(ComputationalRules("8","8","e"));
    C.rules.insert(ComputationalRules("8","4'","f"));

    FiniteAutomata result = A + B;
    QCOMPARE(result, C);
}

void FiniteAutomata_test::simple_iterate_test()
{
    FiniteAutomata simple_C;
    simple_C.alphabet.insert("a");
    simple_C.states.insert("1");
    simple_C.states.insert("2");
    simple_C.states.insert("3");
    simple_C.states.insert("0");
    simple_C.startState = "0";
    simple_C.finalStates.insert("3");
    simple_C.rules.insert(ComputationalRules("1","2","a"));
    simple_C.rules.insert(ComputationalRules("0","1",EPSILON));
    simple_C.rules.insert(ComputationalRules("2","3",EPSILON));
    simple_C.rules.insert(ComputationalRules("2","1",EPSILON));
    simple_C.rules.insert(ComputationalRules("0","3",EPSILON));
    //simple_result.rules.insert(ComputationalRules("1","2",EPSILON));

    FiniteAutomata simple_result = FiniteAutomata::iteration(simpleFA);
    QCOMPARE(simple_C, simple_result);
}

void FiniteAutomata_test::iterate_test()
{
    FiniteAutomata C;
    C.alphabet.insert("a");
    C.alphabet.insert("b");
    C.alphabet.insert("c");
    C.alphabet.insert("d");
    C.states.insert("0");
    C.states.insert("1");
    C.states.insert("2");
    C.states.insert("3");
    C.states.insert("4");
    C.states.insert("5");
    C.states.insert("6");
    C.startState = "0";
    C.finalStates.insert("6");
    C.rules.insert(ComputationalRules("1","2","a"));
    C.rules.insert(ComputationalRules("2","4","b"));
    C.rules.insert(ComputationalRules("4","1","a"));
    C.rules.insert(ComputationalRules("3","5","a"));
    C.rules.insert(ComputationalRules("4","4","c"));
    C.rules.insert(ComputationalRules("5","3","d"));

    //****************epsilon rules********************
    //new start to old start
    C.rules.insert(ComputationalRules("0","1",EPSILON));
    //old ends to new end
    C.rules.insert(ComputationalRules("3","6",EPSILON));
    C.rules.insert(ComputationalRules("4","6",EPSILON));
    C.rules.insert(ComputationalRules("5","6",EPSILON));
    //new start to new end
    C.rules.insert(ComputationalRules("0","6",EPSILON));
    //old ends to old start
    C.rules.insert(ComputationalRules("3","1",EPSILON));
    C.rules.insert(ComputationalRules("4","1",EPSILON));
    C.rules.insert(ComputationalRules("5","1",EPSILON));

    FiniteAutomata result = FiniteAutomata::iteration(A);

    QCOMPARE(result, C);




}

void FiniteAutomata_test::cleanupTestCase()
{
}
