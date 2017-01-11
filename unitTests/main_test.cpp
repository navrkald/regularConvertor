#include <QtTest/QtTest>
#include "finite_machine/finiteautomata_test.h"
#include "algorithms/regexptofa_test.h"
#include "algorithms/removeepsilon_test.h"
#include "algorithms/fatodfa_test.h"
#include "algorithms/dfatominfa_test.h"
#include <CFG/cfg_test.h>
int main()

{

  FiniteAutomata_test test1;
  QTest::qExec(&test1);

  RegExpToFA_test test2;
  QTest::qExec(&test2);

  RemoveEpsilon_test test3;
  QTest::qExec(&test3);

  FaToDFA_test test4;
  QTest::qExec(&test4);

  DFAtoMinFA_test test5;
  QTest::qExec(&test5);


  CfgTest test6;
  QTest::qExec(&test6);
  return 0;

}


