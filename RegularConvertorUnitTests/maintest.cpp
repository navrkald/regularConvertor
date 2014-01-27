#include <QtTest/QtTest>
#include "finite_machine/finiteautomata_test.h"
#include "algorithms/regexptofa_test.h"
#include "algorithms/removeepsilon_test.h"

int main()

{

  FiniteAutomata_test test1;
  QTest::qExec(&test1);


  RegExpToFA_test test2;
  QTest::qExec(&test2);

  RemoveEpsilon_test test3;
  QTest::qExec(&test3);

  return 0;

}


