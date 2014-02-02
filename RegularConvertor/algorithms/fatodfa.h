#ifndef FATODFA_H
#define FATODFA_H

#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
class FaToDFA
{
public:
    FaToDFA(FiniteAutomata _FA);
    FiniteAutomata getDFA();
    QString qsetToQstring(QSet<QString> set);
private:
    FiniteAutomata FA;
};

#endif // FATODFA_H
