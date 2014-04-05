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

#define CHECK_STEP_TIMEOUT 1000

class RegExpToFA : public Algorithm
{
    Q_OBJECT

public:
    RegExpToFA(RegExp* _re, modes _mode = NONE);
    RegExpToFA(AlgorithmWidget* _algorithm_widget,modes _mode, RegExpWidget* _re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget, RegExp* _re = new RegExp(), QObject* parrent = 0);
    void computeSolution();

    bool continue_running;
    int num;
    QList <RegExpNode*> getAvailableNodes();
    RegExp* re;
    typedef struct step
    {
        int num;
        int actInstruction;
        RegExp* re;
    } steps;

    QList<steps> history;
    int actPos;


    void initInstructions();
//    void setNewRegExp(RegExp* re);

public slots:
    void setMode(Algorithm::modes _mode, RegExp* _re = new RegExp());
//    void setRE_old(RegExp* _re);
    void setRE(RegExp* _re);
    void selectRegExp(QModelIndex index);
    void saveStep();
    void runAlgorithm(int mil_sec);
    void nextStep();
    void prewStep();
    void stop();
    void getData(QModelIndex _index);
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();
    void setExample(RegExp* _re);

private:
    QTimer *play_timer;
    QTimer *check_step_timer;
    AlgorithmWidget* algorithm_widget;
    modes mode;
    RegExpWidget* re_widget;
    FA_widget* left_fa_widget;
    FA_widget* center_fa_widget;
    FA_widget* right_fa_widget;
    QList<RegExpNode*> nodesToProcede;

    void removeFuture();
    RegExpNode* chooseRandomNode();
    void postOrder(RegExpNode* node);

};

#endif // REGEXPTOFA_H
