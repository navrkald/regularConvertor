#ifndef DFATOMINFA_H
#define DFATOMINFA_H

#include "finite_machine/finiteautomata.h"
#include "set/set_of_sets.h"

class DFAtoMinFA
{
public:
    DFAtoMinFA(FiniteAutomata _FA);
    FiniteAutomata FA;
    FiniteAutomata convert();

private:
    bool canDivide(QString symbol, QSet< QSet <QString> > Qm, QSet<QString> X, QSet <QString> &X1, QSet <QString> &X2);
    void doFision(QSet<QSet<QString> > &Qm, QSet<QString> X, QSet<QString> X1, QSet<QString> X2);
};

#endif // DFATOMINFA_H
