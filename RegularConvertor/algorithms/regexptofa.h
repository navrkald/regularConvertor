#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H
#include "algorithm.h"
#include "reg_exp/regexp.h"
#include "finite_machine/finiteautomata.h"
#include "finite_machine/fa_widget.h"
#include "reg_exp/regexpwidget.h"
#include "algorithms/algorithmwidget.h"
#include <stdlib.h>
#include <time.h>
#include <QtTest/QTest>

class RegExpToFA : public Algorithm
{
    Q_OBJECT

public:
    RegExpToFA(RegExp* _re);
    RegExpToFA(AlgorithmWidget* _algorithm_widget,modes _mode, RegExpWidget* _re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget);
    void computeSolution();

    bool continue_running;

    QList <RegExpNode*> getAvailableNodes();
    RegExp* re;

public slots:
    void setRE(RegExp* _re);
    void selectRegExp(QModelIndex index);

    void runAlgorithm(int mil_sec);
    void nextStep();
    void prewStep();
    void stop();

private:
    QTimer *timer;

    AlgorithmWidget* algorithm_widget;
    modes mode;
    RegExpWidget* re_widget;
    FA_widget* left_fa_widget;
    FA_widget* center_fa_widget;
    FA_widget* right_fa_widget;

    QList<RegExpNode*> nodesToProcede;
    RegExpNode* chooseRandomNode();
    void postOrder(RegExpNode* node);

};

#endif // REGEXPTOFA_H
