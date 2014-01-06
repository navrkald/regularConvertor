#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H
#include "algorithm.h"
#include "reg_exp/regexp.h"
#include "finite_machine/finiteautomata.h"

class RegExpToFA : public algorithm
{
public:
    RegExpToFA(RegExp _re);
    void computeSolution();
    QList <RegExpNode*> getAvailableNodes();
    RegExpNode* chooseNode();
    RegExp re;
};

#endif // REGEXPTOFA_H
