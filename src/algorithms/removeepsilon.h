#ifndef REMOVEEPSILON_H
#define REMOVEEPSILON_H

#include "finite_machine/finiteautomata.h"
#include "algorithm.h"
#include "widgets/algorithmwidget.h"
#include "widgets/fawidget.h"
#include "algorithms/epsiloncloserwidget.h"
#include "set/set_of_sets.h"
#include "algorithms/epsiloncloserwidget.h"
#include "finite_machine/multiselectcompleter.h"
#include <widgets/variableswidget.h>

class RemoveEpsilon : public CAlgorithm
{
    Q_OBJECT
public:
    explicit RemoveEpsilon(QObject *parrent = 0) : CAlgorithm(parrent) {}
    explicit RemoveEpsilon(FiniteAutomata _FA);
    explicit RemoveEpsilon(AlgorithmModes _mode, CAlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, CVariablesWidget* _var_widget, QListWidget* _epsilon_closer_list_widget, QObject* parrent = 0);
    void Init(CAlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, CVariablesWidget* _var_widget, QListWidget* _epsilon_closer_list_widget);
    FiniteAutomata computeSolution();
    virtual void InitInstructions();



    typedef struct step
    {
        int num;
        int actInstruction;
        int prewInstruction;
        FiniteAutomata non_epsilon_FA;
//        QListWidget* epsilon_closer_list_widget;
//        QObject* parrent;
        QString p;
        QString p_prime;  // p' = "p prime"
        ComputationalRules non_epsilon_rule;
        ComputationalRules non_epsilon_prime_rule;
        QList<QString> epsilon_closer;
        QList< QPair <QString ,QList<QString> > >  epsilon_closers;
        QList<QString> p_list;
        QList<QString> p_prime_list;
        QList<ComputationalRules> non_epsilon_rule_list;
    } steps;

    QList<steps> history;
    void SetInputFA(FiniteAutomata _FA);
    void SetOutputFA(FiniteAutomata out_FA);
    const FiniteAutomata& GetInputFA();
    const FiniteAutomata& GetOutputFA();

    FiniteAutomata FA;
    FiniteAutomata non_epsilon_FA;
public slots:
    void setFA(FiniteAutomata* FA);
    void set_not_epsilonFA(FiniteAutomata* FA);
    void SetMode(AlgorithmModes _mode);
    void saveStep();
    //void runAlgorithm(int mil_sec);
    void nextStep();
    void prevStep();
    //void stop();
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();
    void toBegin();
    void toEnd();

private:
    FiniteAutomata correct_FA;
    FiniteAutomata backup_FA;
    CAlgorithmWidget* m_algorithmWidget;
    AlgorithmModes mode;
    FA_widget* epsilon_fa_widget;
    FA_widget* not_epsilon_fa_widget;
    CVariablesWidget* var_widget;
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
    QList< QPair <QString ,QList<QString> > >  epsilon_closers;
    QList< QPair <QString ,QList<QString> > >  correct_epsilon_closers;


    //internal functions
    inline QString p_toString() {return QString("<b>p</b> = %1").arg(p);}
    inline QString p_prime_toString() {return QString("<b>p'</b> = %1").arg(p_prime);}
    inline QString non_epsilon_rule_toString() {return QString("<b>r</b> = {%1}").arg(non_epsilon_rule.toString());}
    inline QString non_epsilon_prime_rule_toString() {return QString("<b>r'</b> = {%1}").arg(non_epsilon_prime_rule.toString());}
    inline QString epsilon_closer_toString() {return QString(tr("ε-closer(<b>%1</b>) = %2")).arg(p,qSetToQString(epsilon_closer.toSet()));}
    inline QString F_toString(){return QString("<b>F</b> = %1").arg(qSetToQString(FA.m_finalStates));}
    inline QString F_prime_toString(){return QString("<b>F'</b> = %1").arg(qSetToQString(non_epsilon_FA.m_finalStates));}
    inline QString symbol_toString(){return QString("<b>a</b> = %1").arg(non_epsilon_rule.symbol);}
    inline QString q_toString(){return QString("<b>q</b> = %1").arg(non_epsilon_rule.to);}
    void setEpsilonCloserWidget();
    void prepareGUItoCheck();
    void RemoveFuture();
    void showVariables();
    void clearVariables();
};

#endif // REMOVEEPSILON_H
