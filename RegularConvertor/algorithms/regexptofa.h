#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H
#include "algorithm.h"
#include "reg_exp/regexp.h"
#include "finite_machine/finiteautomata.h"
#include "finite_machine/fa_widget.h"
#include "reg_exp/regexpwidget.h"


class RegExpToFA : public Algorithm
{
public:
    RegExpToFA(RegExp _re);
    RegExpToFA(RegExpWidget* re_widget, FA_widget* left_fa_widget, FA_widget* center_fa_widget, FA_widget* right_fa_widget);
    void computeSolution();
    QList <RegExpNode*> getAvailableNodes();
    RegExpNode* chooseRandomNode();
    RegExp re;
private:
    void postOrder(RegExpNode* node);
    QList<RegExpNode*> nodesToProcede;
};

#endif // REGEXPTOFA_H
