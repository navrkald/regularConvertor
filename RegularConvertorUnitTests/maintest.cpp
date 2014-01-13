#include <QtTest/QtTest>
#include "finite_machine/finiteautomata_test.h"

int main()

{

  FiniteAutomata_test FA;

  QTest::qExec(&FA);



  //SquareTest sq;

  //QTest::qExec(&sq);



  return 0;

}


