#ifndef FATODFA_H
#define FATODFA_H

#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
#include "set/set_of_sets.h"
#include "algorithm.h"
#include "set/set_of_sets.h"
#include "finite_machine/finiteautomata.h"
#include "algorithm.h"
#include "widgets/algorithmwidget.h"
#include <widgets/fa_widget.h>
#include <widgets/variableswidget.h>

class FaToDFA : public CAlgorithm
{
    Q_OBJECT

public:
    FaToDFA(modes mode, CAlgorithmWidget *_algorithm_widget, FA_widget *_not_dfa_widget, FA_widget *_dfa_widget, CVariablesWidget *_var_widget, QObject *parrent);
    FaToDFA(QObject *parrent = 0) : CAlgorithm(parrent) {}
    void Init(CAlgorithmWidget *_algorithm_widget, FA_widget *_not_dfa_widget, FA_widget *_dfa_widget, CVariablesWidget *_var_widget, QObject *parrent);
    FaToDFA(FiniteAutomata _FA);
    ~FaToDFA();
    FiniteAutomata computeSolution();
    virtual void InitInstructions();



    void setInputFA(FiniteAutomata& _FA);
    void setOutputFA(FiniteAutomata out_FA);

    const FiniteAutomata& GetInputFA();
    const FiniteAutomata& GetOutputFA();


    FiniteAutomata FA;
    FiniteAutomata DFA;

public slots:
    void SetMode(CAlgorithm::modes _mode);
    void setFA(FiniteAutomata* _FA);
    void setDFA(FiniteAutomata* _FA);
    void prevStep();
    void nextStep();
    void checkSolution();
    void showCorrectSolution();
    void showUserSolution();
    void toBegin();
    void toEnd();

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

    QList<steps> m_history;
    FiniteAutomata m_backup_FA;
    FiniteAutomata m_correct_FA;
    modes m_mode;
    CAlgorithmWidget* m_algorithm_widget;
    FA_widget* m_not_dfa_widget;
    FA_widget* m_dfa_widget;
    CVariablesWidget* m_var_widget;

    //internal variables
    QSet <QString> m_act_state;       // = Q'
    QString m_act_state_name;
    QSet<QString> m_discovered_state; // = Q''
    QSet< QSet<QString> > m_Q_new;
    QString m_a;
    QString m_p;
    QString m_q;
    ComputationalRules m_r;
    ComputationalRules m_r_prime;
    QStringList m_alphabet;
    QSet<ComputationalRules> m_rules;
    QSet<ComputationalRules> m_rules_prime;

    // internal methods
    void RemoveFuture();
    void clearVariables();
    void showVariables();
    void saveStep();
    //
    // to string function, using when printing variables
    //
    inline QString varToString(QString var_name, QString str){return QString("<b>%1</b> = %2").arg(var_name,str);}
    inline QString varToString(QString var_name, QSet <QString> set){return QString("<b>%1</b> = %2").arg(var_name,qSetToQString(set));}
    inline QString varToString(QString var_name, QSet< QSet<QString> > set_of_sets){return QString("<b>%1</b> = %2").arg(var_name,qSetToQString(setOfSubsetsToSet(set_of_sets)));}
};

#endif // FATODFA_H
