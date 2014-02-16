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

    epsilon_FA.alphabet = QSet<QString>() << "a"<<"b"<<"c";
    epsilon_FA.states = QSet<QString>()<<"0"<<"0\'"<<"0'\'"<<"1"<<"1\'"<<"1'\'"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    epsilon_FA.startState = "6";
    epsilon_FA.finalStates = QSet<QString>()<<"7";

    epsilon_FA.rules.insert(ComputationalRules("0","1","a"));
    epsilon_FA.rules.insert(ComputationalRules("0\'","1\'","b"));
    epsilon_FA.rules.insert(ComputationalRules("1","0\'",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("1\'","0",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("2","0",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("1\'","3",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("2","3",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("0\''","1\''","c"));
    epsilon_FA.rules.insert(ComputationalRules("4","2",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("4","0\''",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("3","5",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("1\''","5",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("6","4",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("5","7",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("5","4",EPSILON));
    epsilon_FA.rules.insert(ComputationalRules("6","7",EPSILON));

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

    unreachabe_states_FA.alphabet << "a" << "b";
    unreachabe_states_FA.states << "s" << "q1" << "q2" << "f";
    unreachabe_states_FA.startState = "s";
    unreachabe_states_FA.finalStates << "f" << "q2";
    unreachabe_states_FA.addRule("s","q1","a");
    unreachabe_states_FA.addRule("q2","q1","a");
    unreachabe_states_FA.addRule("q2","s","b");
    unreachabe_states_FA.addRule("q1","f","b");


   nonterminating_states_FA.alphabet << "a" << "b";
   nonterminating_states_FA.states << "s" << "q1" << "q2" << "q3" << "f";
   nonterminating_states_FA.startState = "s";
   nonterminating_states_FA.finalStates << "f" << "q2";
   nonterminating_states_FA.addRule("s","q1","a");
   nonterminating_states_FA.addRule("q1","q2","a");
   nonterminating_states_FA.addRule("q1","q3","b");
   nonterminating_states_FA.addRule("q1","f","b");

   not_makeWellDefined_FA.alphabet << "a" << "b" << "c";
   not_makeWellDefined_FA.states << "s" << "f";
   not_makeWellDefined_FA.startState = "s";
   not_makeWellDefined_FA.finalStates << "f";
   not_makeWellDefined_FA.addRule("s", "s", "a");
   not_makeWellDefined_FA.addRule("s", "f", "b");
   not_makeWellDefined_FA.addRule("f", "f", "c");

    //
    // Check variables initialise
    //
    concatenate_check.alphabet = QSet<QString>() << "a"<<"b"<<"c"<<"d"<<"e"<<"f";
    concatenate_check.states = QSet<QString>()<<"1"<<"2"<<"3"<<"4"<<"4'"<<"5"<<"5'"<<"6"<<"7"<<"8"<<"9"<<"10";
    concatenate_check.startState = "1";
    concatenate_check.finalStates = QSet<QString>()<<"8"<<"9"<<"10";
    //A rules
    concatenate_check.rules.insert(ComputationalRules("1","2","a"));
    concatenate_check.rules.insert(ComputationalRules("2","4","b"));
    concatenate_check.rules.insert(ComputationalRules("4","1","a"));
    concatenate_check.rules.insert(ComputationalRules("3","5","a"));
    concatenate_check.rules.insert(ComputationalRules("4","4","c"));
    concatenate_check.rules.insert(ComputationalRules("5","3","d"));
    //epsilon rules
    concatenate_check.rules.insert(ComputationalRules("3","4'",EPSILON));
    concatenate_check.rules.insert(ComputationalRules("4","4'",EPSILON));
    concatenate_check.rules.insert(ComputationalRules("5","4'",EPSILON));
    //B rules
    concatenate_check.rules.insert(ComputationalRules("4'","6","c"));
    concatenate_check.rules.insert(ComputationalRules("6","5'","d"));
    concatenate_check.rules.insert(ComputationalRules("5'","9","e"));
    concatenate_check.rules.insert(ComputationalRules("4'","4'","e"));
    concatenate_check.rules.insert(ComputationalRules("9","7","f"));
    concatenate_check.rules.insert(ComputationalRules("7","10","c"));
    concatenate_check.rules.insert(ComputationalRules("10","4'","d"));
    concatenate_check.rules.insert(ComputationalRules("10","8","d"));
    concatenate_check.rules.insert(ComputationalRules("8","8","e"));
    concatenate_check.rules.insert(ComputationalRules("8","4'","f"));

    alternate_check.alphabet = QSet<QString>() << "a"<<"b"<<"c"<<"d"<<"e"<<"f";
    alternate_check.states = QSet<QString>()<<"0"<<"1"<<"2"<<"3"<<"4"<<"4'"<<"5"<<"5'"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11";
    alternate_check.startState = "0";
    alternate_check.finalStates = QSet<QString>()<<"11";
    //A rules
    alternate_check.rules.insert(ComputationalRules("1","2","a"));
    alternate_check.rules.insert(ComputationalRules("2","4","b"));
    alternate_check.rules.insert(ComputationalRules("4","1","a"));
    alternate_check.rules.insert(ComputationalRules("3","5","a"));
    alternate_check.rules.insert(ComputationalRules("4","4","c"));
    alternate_check.rules.insert(ComputationalRules("5","3","d"));
    //epsilon rules
    //new start to old starts
    alternate_check.rules.insert(ComputationalRules("0","1",EPSILON));
    alternate_check.rules.insert(ComputationalRules("0","4'",EPSILON));
    //old ends o new end
    alternate_check.rules.insert(ComputationalRules("8","11",EPSILON));
    alternate_check.rules.insert(ComputationalRules("9","11",EPSILON));
    alternate_check.rules.insert(ComputationalRules("10","11",EPSILON));
    alternate_check.rules.insert(ComputationalRules("3","11",EPSILON));
    alternate_check.rules.insert(ComputationalRules("4","11",EPSILON));
    alternate_check.rules.insert(ComputationalRules("5","11",EPSILON));
    //B rules
    alternate_check.rules.insert(ComputationalRules("4'","6","c"));
    alternate_check.rules.insert(ComputationalRules("6","5'","d"));
    alternate_check.rules.insert(ComputationalRules("5'","9","e"));
    alternate_check.rules.insert(ComputationalRules("4'","4'","e"));
    alternate_check.rules.insert(ComputationalRules("9","7","f"));
    alternate_check.rules.insert(ComputationalRules("7","10","c"));
    alternate_check.rules.insert(ComputationalRules("10","4'","d"));
    alternate_check.rules.insert(ComputationalRules("10","8","d"));
    alternate_check.rules.insert(ComputationalRules("8","8","e"));
    alternate_check.rules.insert(ComputationalRules("8","4'","f"));

    simple_iterate_check.alphabet.insert("a");
    simple_iterate_check.states.insert("1");
    simple_iterate_check.states.insert("2");
    simple_iterate_check.states.insert("3");
    simple_iterate_check.states.insert("0");
    simple_iterate_check.startState = "0";
    simple_iterate_check.finalStates.insert("3");
    simple_iterate_check.rules.insert(ComputationalRules("1","2","a"));
    simple_iterate_check.rules.insert(ComputationalRules("0","1",EPSILON));
    simple_iterate_check.rules.insert(ComputationalRules("2","3",EPSILON));
    simple_iterate_check.rules.insert(ComputationalRules("2","1",EPSILON));
    simple_iterate_check.rules.insert(ComputationalRules("0","3",EPSILON));

    iterate_check.alphabet.insert("a");
    iterate_check.alphabet.insert("b");
    iterate_check.alphabet.insert("c");
    iterate_check.alphabet.insert("d");
    iterate_check.states.insert("0");
    iterate_check.states.insert("1");
    iterate_check.states.insert("2");
    iterate_check.states.insert("3");
    iterate_check.states.insert("4");
    iterate_check.states.insert("5");
    iterate_check.states.insert("6");
    iterate_check.startState = "0";
    iterate_check.finalStates.insert("6");
    iterate_check.rules.insert(ComputationalRules("1","2","a"));
    iterate_check.rules.insert(ComputationalRules("2","4","b"));
    iterate_check.rules.insert(ComputationalRules("4","1","a"));
    iterate_check.rules.insert(ComputationalRules("3","5","a"));
    iterate_check.rules.insert(ComputationalRules("4","4","c"));
    iterate_check.rules.insert(ComputationalRules("5","3","d"));
    //****************epsilon rules********************
    //new start to old start
    iterate_check.rules.insert(ComputationalRules("0","1",EPSILON));
    //old ends to new end
    iterate_check.rules.insert(ComputationalRules("3","6",EPSILON));
    iterate_check.rules.insert(ComputationalRules("4","6",EPSILON));
    iterate_check.rules.insert(ComputationalRules("5","6",EPSILON));
    //new start to new end
    iterate_check.rules.insert(ComputationalRules("0","6",EPSILON));
    //old ends to old start
    iterate_check.rules.insert(ComputationalRules("3","1",EPSILON));
    iterate_check.rules.insert(ComputationalRules("4","1",EPSILON));
    iterate_check.rules.insert(ComputationalRules("5","1",EPSILON));

    //Epsilon closers check values
    epsilonCloser_2_check << "2" << "0" << "3" << "5" << "7" << "4" << "0''";
    epsilonCloser_6_check << "6" << "4" << "7" << "2" << "0" << "0''" << "3" << "5";
    epsilonCloser_3_check << "3" << "5" << "7" << "4" << "2" << "0" << "0''";

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

    unreachabe_states_check_FA.alphabet << "a" << "b";
    unreachabe_states_check_FA.states << "s" << "q1" << "f";
    unreachabe_states_check_FA.startState = "s";
    unreachabe_states_check_FA.finalStates << "f";
    unreachabe_states_check_FA.addRule("s","q1","a");
    unreachabe_states_check_FA.addRule("q1","f","b");


    nonterminating_states_check_FA.alphabet << "a" << "b";
    nonterminating_states_check_FA.states << "s" << "q1" << "q2" << "f";
    nonterminating_states_check_FA.startState = "s";
    nonterminating_states_check_FA.finalStates << "f" << "q2";
    nonterminating_states_check_FA.addRule("s","q1","a");
    nonterminating_states_check_FA.addRule("q1","f","b");
    nonterminating_states_check_FA.addRule("q1","q2","a");

    makeWellDefined_check_FA.alphabet << "a" << "b" << "c";
    makeWellDefined_check_FA.states << "s" << "f" << "0";
    makeWellDefined_check_FA.startState = "s";
    makeWellDefined_check_FA.finalStates << "f";
    makeWellDefined_check_FA.addRule("s", "s", "a");
    makeWellDefined_check_FA.addRule("s", "f", "b");
    makeWellDefined_check_FA.addRule("s", "0", "c");
    makeWellDefined_check_FA.addRule("f", "f", "c");
    makeWellDefined_check_FA.addRule("f", "0", "b");
    makeWellDefined_check_FA.addRule("f", "0", "a");

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
    toMinFA_FA1_check.states << "{f, s}" << "{q1, q2}" << "{q3, q4}";
    toMinFA_FA1_check.startState = "{f, s}";
    toMinFA_FA1_check.finalStates << "{f, s}";
    toMinFA_FA1_check.addRule("{f, s}", "{f, s}", "a");
    toMinFA_FA1_check.addRule("{q1, q2}", "{q1, q2}", "a");
    toMinFA_FA1_check.addRule("{q3, q4}", "{q3, q4}", "a");
    toMinFA_FA1_check.addRule("{q1, q2}", "{f, s}", "b");
    toMinFA_FA1_check.addRule("{f, s}", "{q3, q4}", "b");
    toMinFA_FA1_check.addRule("{q3, q4}", "{q1, q2}", "b");

    normalize_FA1.alphabet << "a" << "b";
    normalize_FA1.states << "q0" <<"q1" << "q2" << "q3" << "q4";
    normalize_FA1.startState = "q0";
    normalize_FA1.finalStates << "q2";

    normalize_FA1_check;
}

void FiniteAutomata_test::concatenate_test()
{
    FiniteAutomata result = FiniteAutomata::concatenate(A,B);
    QCOMPARE(result, concatenate_check);
}

void FiniteAutomata_test::alternate_test()
{
    FiniteAutomata result = A + B;
    QCOMPARE(result, alternate_check);
}

void FiniteAutomata_test::simple_iterate_test()
{
    FiniteAutomata simple_result = FiniteAutomata::iteration(simpleFA);
    QCOMPARE(simple_iterate_check, simple_result);
}

void FiniteAutomata_test::iterate_test()
{
    FiniteAutomata result = FiniteAutomata::iteration(A);
    QCOMPARE(result, iterate_check);
}

void FiniteAutomata_test::epsilonCloser_test()
{
    QSet <QString> result1 = epsilon_FA.epsilonCloser("2");
    QCOMPARE(result1, epsilonCloser_2_check);

    QSet <QString> result2 = epsilon_FA.epsilonCloser("6");
    QCOMPARE(result2, epsilonCloser_6_check);

    QSet <QString> result3 = epsilon_FA.epsilonCloser("3");
    QCOMPARE(result3, epsilonCloser_3_check);
}

void FiniteAutomata_test::remove_epsilon_FA1_test()
{
    //TODO vymyslet priklad kde nebudou na konci po prevodu stejne koncove stavy
    FiniteAutomata result = remove_epsilon_FA1.removeEpsilon(remove_epsilon_FA1);
    QCOMPARE(remove_epsilon_FA1_check, result);
}

void FiniteAutomata_test::toDFA_test()
{
    FiniteAutomata result = toDFA_FA1.toDFA(toDFA_FA1);;
    QCOMPARE(toDFA_FA1_check, result);
}

void FiniteAutomata_test::removeUnreachableStates_test()
{
    FiniteAutomata result = unreachabe_states_FA.removeUnreachableStates(unreachabe_states_FA);
    QCOMPARE(unreachabe_states_check_FA, result);
}

void FiniteAutomata_test::removeNonTerminatingStates_test()
{
    FiniteAutomata result = nonterminating_states_FA.removeNonTerminatingStates(nonterminating_states_FA);
    QCOMPARE(nonterminating_states_check_FA, result);
}

void FiniteAutomata_test::makeWellDefined_test()
{
    FiniteAutomata result = not_makeWellDefined_FA.makeWellDefined(not_makeWellDefined_FA);
    QCOMPARE(makeWellDefined_check_FA, result);
}

void FiniteAutomata_test::toMinFA_test()
{
    FiniteAutomata result = toMinFA_FA1.toMinFA(toMinFA_FA1);
    QCOMPARE (result, toMinFA_FA1_check);
}

void FiniteAutomata_test::normalize()
{

}

void FiniteAutomata_test::cleanupTestCase()
{
}
