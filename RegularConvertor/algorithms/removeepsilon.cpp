#include "removeepsilon.h"

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"

#define HEADER 0
#define FOREACH_P_EPSILONCLOSER 1
#define EPSILONCLOSER 2
#define FOREACH_P 3
#define FOREACH_P_IN_CLOSER 4
#define FOREACH_RULE 5
#define NEW_RULE 6
#define FOREACH_RULE_P_FINAL 7
#define IF_FINAL 8
#define NEW_FINAL 9

RemoveEpsilon::RemoveEpsilon(FiniteAutomata _FA)
{
    this->FA = _FA;
}

RemoveEpsilon::RemoveEpsilon(FiniteAutomata _FA, modes _mode, AlgorithmWidget* _algorithm_widget, FA_widget* _epsilon_fa_widget, FA_widget* _not_epsilon_fa_widget, QTextEdit* _var_widget, QListWidget* _epsilon_closer_list_widget, QObject* parrent)
 : FA(_FA), Algorithm(parrent), algorithm_widget(_algorithm_widget), mode(_mode), epsilon_fa_widget(_epsilon_fa_widget), not_epsilon_fa_widget(_not_epsilon_fa_widget), var_widget(_var_widget), epsilon_closer_list_widget(_epsilon_closer_list_widget)
{
    actInstruction = HEADER;
    prewInstruction = HEADER;
    instruction_count = NEW_FINAL+1;

    initInstructions();
    initBreakpoints(instruction_count);

    QIcon epsilon_closer_icon = QIcon(":/algorithms/algorithms/pictures/epsilon_closer.png");
    QIcon remove_epsilon_icon = QIcon(":/algorithms/algorithms/pictures/remove_epsilon.png");

    this->setColumnCount(1);
    this->setRowCount(instructions.count());

    var_widget->setText("");

    for(int i = 0; i < instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,instructions[i],Qt::EditRole);
        setData(index,true,Algorithm::HasBreakpoint_Role);
        setData(index,false,Algorithm::Breakpoint_Role);
        switch(i)
        {
            case HEADER:
                setData(index,false,Algorithm::HasBreakpoint_Role);
                break;
            case EPSILONCLOSER:
                setData(index,epsilon_closer_icon,Qt::DecorationRole);
                break;
            case NEW_RULE:
                setData(index,remove_epsilon_icon,Qt::DecorationRole);
                break;
        }
    }
}

void RemoveEpsilon::initInstructions()
{
    instructions.resize(instruction_count);
    instructions[HEADER] =                      "<b>Vstup:</b> KA <i>M</i>=(Q,Σ,R,s,F)<br><b>Výstup:</b> KA bez ε-přechodů M'=(Q,Σ,R',s,F')";
    instructions[FOREACH_P_EPSILONCLOSER] =    "<b>for each</b> p ∈ Q <b>do</b>";
    instructions[EPSILONCLOSER] =              INDENT "ε-uzávěr(p) = {q:q∈Q, p ⊢* q}";

    instructions[FOREACH_P] =                  "<b>for each</b> p ∈ Q <b>do</b>";
    instructions[FOREACH_P_IN_CLOSER] =         INDENT      "<b>for each</b> p'∈ ε-uzávěr(p) <b>do</b>";
    instructions[FOREACH_RULE] =                INDENT INDENT      "<b>for each</b> r={p'a→q}∈ R <b>where</b> a ∈ Σ <b>do</b>";
    instructions[NEW_RULE] =                    INDENT INDENT INDENT      "R'∈ R' ∪ r'={pa→q}";

    instructions[FOREACH_RULE_P_FINAL] =         "<b>for each</b> p ∈ Q <b>do</b>";
    instructions[IF_FINAL] =                     INDENT       "<b>if</b> ε-uzávěr(p) ∩ F ≠ ∅ <b>then</b>";
    instructions[NEW_FINAL] =                    INDENT INDENT       "F'∈ F' ∪ {p}";
}



void RemoveEpsilon::nextStep()
{
    switch(prewInstruction)
    {
        case HEADER:
            p_list = FA.get_sorted_states();
            actInstruction = FOREACH_P_EPSILONCLOSER;
        break;
    }

    switch(actInstruction)
    {
        case FOREACH_P_EPSILONCLOSER:
            p = p_list.first();
            p_list.pop_front();
        case EPSILONCLOSER:
            epsilon_closer = FA.epsilonCloser(p).toList();
            qSort(epsilon_closer);
            QListWidgetItem * item = new QListWidgetItem(epsilon_closer_list_widget);
            epsilon_closer_list_widget->setItemWidget(item,new EpsilonCloserWidget(p,epsilon_closer,epsilon_closer_list_widget));
        break;
    }
    showVariables();
    prewInstruction = actInstruction;
//    QString p;
//    QString p_prime;  // p' = "p prime"
//    ComputationalRules non_epsilon_rule;
//    QList<QString> p_list;
//    QList<QString> p_prime_list;
//    QList<ComputationalRules> non_epsilon_rule_list;
}

void RemoveEpsilon::saveStep()
{

}

void RemoveEpsilon::runAlgorithm(int mil_sec)
{

}

void RemoveEpsilon::showVariables()
{
    var_widget->clear();
    QString text;
    switch(actInstruction)
    {
        case HEADER:
            text = "";
            break;

        case FOREACH_P_EPSILONCLOSER:
            text = p_toString();                        // p =
            break;

        case EPSILONCLOSER:
            text = p_toString();                        // p =
            text += epsilon_closer_toString();          // e-closer(p)
            break;

        case FOREACH_P:
            text = p_toString();                        // p =
            break;

        case FOREACH_P_IN_CLOSER:
            text =  p_toString();                       // p =
            text += epsilon_closer_toString();          // e-closer(p)
            text += p_prime_toString();
            break;
        case FOREACH_RULE:
            text = p_toString();                        // p =
            text += epsilon_closer_toString();          // e-closer(p)
            text += p_prime_toString();                 // p' =
            text += non_epsilon_rule_toString();        // r  =
            break;

        case NEW_RULE:
            text = p_toString();                        // p =
            text += epsilon_closer_toString();          // e-closer(p)
            text += p_prime_toString();                 // p' =
            text += non_epsilon_rule_toString();        // r  =
            text += non_epsilon_prime_rule_toString();  // r' =
            break;

        case FOREACH_RULE_P_FINAL:
            text = p_toString();                        // p =
            break;

        case IF_FINAL:
            text =  p_toString();                       // p =
            text += epsilon_closer_toString();          // e-closer(p)
            text += F_toString();                       // F =
            break;

        case NEW_FINAL:
            text =  p_toString();                       // p =
            text += epsilon_closer_toString();          // e-closer(p) =
            text += F_toString();                       // F =
            text += F_prime_toString();                 // F'=
        break;
    }
    var_widget->setText(text);
}

void RemoveEpsilon::prewStep()
{

}

void RemoveEpsilon::stop()
{

}

void RemoveEpsilon::checkSolution()
{

}

void RemoveEpsilon::showCorrectSolution()
{

}

void RemoveEpsilon::showUserSolution()
{

}

#define FOREACH_RULE_P_FINAL 7
#define IF_FINAL 8
#define NEW_FINAL 9

void RemoveEpsilon::setMode(Algorithm::modes)
{

}

FiniteAutomata RemoveEpsilon::computeSolution()
{
    FiniteAutomata FA_not_epsilon;
    FA_not_epsilon.states = FA.states;
    FA_not_epsilon.alphabet = FA.alphabet;
    FA_not_epsilon.startState = FA.startState;

    //Rules
    foreach(QString state,FA.states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1,epsilon_closer)
        {
            QSet <ComputationalRules> rules = FA.nonEpsilonRulesOf(state1);
            foreach(ComputationalRules rule,rules)
            {
                ComputationalRules newRule = rule;
                newRule.from = state;
                FA_not_epsilon.addRule(newRule);
            }
        }
    }

    //Final states
    foreach(QString state,FA.states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1, epsilon_closer)
        {
            if(FA.finalStates.contains(state1))
            {
                FA_not_epsilon.addFinalState(state);
            }
        }
    }
    return FA_not_epsilon;
}
