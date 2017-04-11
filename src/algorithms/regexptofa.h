#ifndef REGEXPTOFA_H
#define REGEXPTOFA_H
#include "algorithm.h"
#include "reg_exp/regexp.h"
#include "finite_machine/finiteautomata.h"
#include "widgets/fawidget.h"
#include "reg_exp/regexpwidget.h"
#include "widgets/algorithmwidget.h"
#include <stdlib.h>
#include <time.h>
//#include <QtTest/QTest>

class RegExpToFA : public CAlgorithm
{
    Q_OBJECT

public:
    explicit RegExpToFA(QObject* parrent = 0) : CAlgorithm(parrent) {}
    explicit RegExpToFA(RegExp* _re, AlgorithmModes _mode = NONE);
    explicit RegExpToFA(CAlgorithmWidget* _algorithm_widget,AlgorithmModes _mode, CRegExpWidget* _re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget, RegExp* _re = new RegExp(), QObject* parrent = 0);
    void Init(CAlgorithmWidget* _algorithm_widget, CRegExpWidget* _re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget, RegExp* _re = new RegExp());
    void computeSolution();

    bool continue_running;
    QList <RegExpNode*> getAvailableNodes();
    RegExp* re;
    typedef struct step
    {
        int num;
        int actInstruction;
        RegExp* re;
    } steps;

    QList<steps> history;


    virtual void InitInstructions();
//    void setNewRegExp(RegExp* re);

    RegExp *GetInputRegExp();
public slots:
    void SetMode(AlgorithmModes _mode);
//    void setRE_old(RegExp* _re);
    void SetRegExp(RegExp* _re);
    void selectRegExp(QModelIndex index);
    void saveStep();
//    void runAlgorithm(int mil_sec);
    void nextStep();
    void prevStep();
//    void stop();
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();
    void SetInputRegExp(RegExp* _re);
    void toBegin();
    void toEnd();

private:
    CAlgorithmWidget* m_algorithmWidget;
    AlgorithmModes mode;
    CRegExpWidget* re_widget;
    FA_widget* left_fa_widget;
    FA_widget* center_fa_widget;
    FA_widget* right_fa_widget;
    QList<RegExpNode*> nodesToProcede;

    void RemoveFuture();
    RegExpNode* chooseRandomNode();
    void postOrder(RegExpNode* node);

};

#endif // REGEXPTOFA_H
