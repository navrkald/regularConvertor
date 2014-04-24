#ifndef FATODFA_H
#define FATODFA_H

#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
#include "set/set_of_sets.h"
#include "algorithm.h"
#include "set/set_of_sets.h"
#include "finite_machine/finiteautomata.h"
#include "algorithm.h"
#include "algorithms/algorithmwidget.h"
#include "finite_machine/fa_widget.h"

class FaToDFA : public Algorithm
{
    Q_OBJECT

public:
    FaToDFA(FiniteAutomata _FA, modes _mode, AlgorithmWidget *_algorithm_widget, FA_widget *_not_dfa_widget, FA_widget *_dfa_widget, QLabel *_var_widget, QObject *parrent);
    FaToDFA(FiniteAutomata _FA);
    FiniteAutomata computeSolution();
    void initInstructions();

    void setMode(Algorithm::modes _mode);
    void setExample(FiniteAutomata _FA);

public slots:
    void setFA(FiniteAutomata* _FA);
    void setDFA(FiniteAutomata* _FA);
    void prevStep();
    void nextStep();
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();

private:

    typedef struct step
    {
        int num;
        int actInstruction;
        int prewInstruction;
        FiniteAutomata DFA;
        QSet <QString> act_state;
        QSet<QString> discovered_state;
        QSet< QSet<QString> > Q_new;
        QString a;
        QString p;
        QString q;
        ComputationalRules r;
        ComputationalRules r_prime;
        QStringList alphabet;
        QSet<ComputationalRules> rules;
        QSet<ComputationalRules> rules_prime;
    } steps;

    QList<steps> history;

    FiniteAutomata correct_FA;
    modes mode;
    AlgorithmWidget* algorithm_widget;
    FA_widget* not_dfa_widget;
    FA_widget* dfa_widget;
    QLabel* var_widget;

    //internal variables
    FiniteAutomata FA;
    FiniteAutomata DFA;
    QSet <QString> act_state;       // = Q'
    QString act_state_name;
    QSet<QString> discovered_state; // = Q''
    QSet< QSet<QString> > Q_new;
    QString a;
    QString p;
    QString q;
    ComputationalRules r;
    ComputationalRules r_prime;
    QStringList alphabet;
    QSet<ComputationalRules> rules;
    QSet<ComputationalRules> rules_prime;

    // internal methods
    void removeFuture();
    void clearVariables();
    void showVariables();
    void saveStep();
    //
    // to string function, using when printing variables
    //
    inline QString varToString(QString var_name, QString str){return QString("<b>%1</b> = %2").arg(var_name,str);}
    inline QString varToString(QString var_name, QSet <QString> set){return QString("<b>%1</b> = %2").arg(var_name,qsetToQstring(set));}
    inline QString varToString(QString var_name, QSet< QSet<QString> > set_of_sets){return QString("<b>%1</b> = %2").arg(var_name,qsetToQstring(setOfSubsetsToSet(set_of_sets)));}
};

#endif // FATODFA_H
