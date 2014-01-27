#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H
#include "algorithm.h"
#include "reg_exp/regexp.h"
#include "finite_machine/finiteautomata.h"

class RegExpToFA : public Algorithm
{
public:
    RegExpToFA(RegExp _re);
    void computeSolution();
    QList <RegExpNode*> getAvailableNodes();
    RegExpNode* chooseRandomNode();
    RegExp re;
private:
    void postOrder(RegExpNode* node);
    QList<RegExpNode*> nodesToProcede;
};

#endif // REGEXPTOFA_H
