#ifndef REMOVEEPSILON_H
#define REMOVEEPSILON_H

#include "finite_machine/finiteautomata.h"
#include "algorithm.h"
#include "algorithms/algorithmwidget.h"
#include "finite_machine/fa_widget.h"
#include "algorithms/epsiloncloserwidget.h"
#include "set/set_of_sets.h"
#include "algorithms/epsiloncloserwidget.h"

class RemoveEpsilon : public Algorithm
{
    Q_OBJECT
public:
    RemoveEpsilon(FiniteAutomata _FA);
    RemoveEpsilon(FiniteAutomata _FA,modes _mode, AlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, QTextEdit* _var_widget, QListWidget* _epsilon_closer_list_widget, QObject* parrent = 0);
    FiniteAutomata computeSolution();
    void initInstructions();
    void showVariables();
public slots:
    void saveStep();
    void runAlgorithm(int mil_sec);
    void nextStep();
    void prewStep();
    void stop();
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();
private:
    FiniteAutomata FA;
    FiniteAutomata non_epsilon_FA;
    AlgorithmWidget* algorithm_widget;
    modes mode;
    FA_widget* epsilon_fa_widget;
    FA_widget* not_epsilon_fa_widget;
    QTextEdit* var_widget;
    QListWidget* epsilon_closer_list_widget;

    //internal variables
    QString p;
    QString p_prime;  // p' = "p prime"
    ComputationalRules non_epsilon_rule;
    ComputationalRules non_epsilon_prime_rule;
    QList<QString> epsilon_closer;
    QList<QString> p_list;
    QList<QString> p_prime_list;
    QList<ComputationalRules> non_epsilon_rule_list;

    //internal functions
    inline QString p_toString() {return QString("<b>p</b> = %1 \n").arg(p);}
    inline QString p_prime_toString() {return QString("<b>p'</b> = %1 \n").arg(p_prime);}
    inline QString non_epsilon_rule_toString() {return QString("<b>r</b> = {%1} \n").arg(non_epsilon_rule.toString());}
    inline QString non_epsilon_prime_rule_toString() {return QString("<b>r'</b> = {%1} \n").arg(non_epsilon_prime_rule.toString());}
    inline QString epsilon_closer_toString() {return QString("<b>ε-uzaver(%1)</b> = %2 \n").arg(p,qsetToQstring(epsilon_closer.toSet()));}
    inline QString F_toString(){return QString("<b>F</b> = %1 \n").arg(qsetToQstring(FA.finalStates));}
    inline QString F_prime_toString(){return QString("<b>F'</b> = %1 \n").arg(qsetToQstring(non_epsilon_FA.finalStates));}
public slots:
    void setMode(Algorithm::modes);
};

#endif // REMOVEEPSILON_H
