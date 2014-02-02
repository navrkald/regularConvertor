#ifndef FATODFA_H
#define FATODFA_H

#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
#include "set/set_of_sets.h"
class FaToDFA
{
public:
    FaToDFA(FiniteAutomata _FA);
    FiniteAutomata getDFA();

private:
    FiniteAutomata FA;
};

#endif // FATODFA_H
