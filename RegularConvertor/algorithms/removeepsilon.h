#ifndef REMOVEEPSILON_H
#define REMOVEEPSILON_H

#include "finite_machine/finiteautomata.h"
#include "algorithm.h"
#include "algorithms/algorithmwidget.h"
#include "finite_machine/fa_widget.h"

class RemoveEpsilon : public Algorithm
{
    Q_OBJECT
public:
    RemoveEpsilon(FiniteAutomata _FA);
    RemoveEpsilon(FiniteAutomata* _FA,modes _mode, AlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, QTextEdit* _var_widget, QObject* parrent = 0);
    FiniteAutomata removeEpsilon();
    void initInstructions();
private:
    FiniteAutomata FA;
    AlgorithmWidget* algorithm_widget;
    modes mode;
    FA_widget* epsilon_fa_widget;
    FA_widget* not_epsilon_fa_widget;
    QTextEdit* var_widget;

};

#endif // REMOVEEPSILON_H
