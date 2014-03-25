#ifndef REMOVEEPSILON_H
#define REMOVEEPSILON_H

#include "finite_machine/finiteautomata.h"
#include "algorithm.h"

class RemoveEpsilon : public Algorithm
{
public:
    RemoveEpsilon(FiniteAutomata _FA);
    FiniteAutomata removeEpsilon();
    void initInstructions();
private:
    FiniteAutomata FA;

};

#endif // REMOVEEPSILON_H
