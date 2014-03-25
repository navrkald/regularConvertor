#ifndef FATODFA_H
#define FATODFA_H

#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
#include "set/set_of_sets.h"
#include "algorithm.h"

class FaToDFA : public Algorithm
{
public:
    FaToDFA(FiniteAutomata _FA);
    FiniteAutomata getDFA();
    void initInstructions();
private:
    FiniteAutomata FA;
};

#endif // FATODFA_H
