#include "fatodfa.h"

FaToDFA::FaToDFA(FiniteAutomata _FA) : FA(_FA)
{
    ;
}

uint qHash(const QSet<QString>) {
  uint seed = 0;

//  for(auto x : c) {
//    seed ^= qHash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//  }

  return seed;
}

QString FaToDFA::qsetToQstring(QSet<QString> set)
{
    QList <QString> list = set.toList();
    qSort(list);
    //reverse list
    //for(int k = 0; k < (list.size()/2); k++) list.swap(k,list.size()-(1+k));

    QString string = "{";
    foreach(QString s,list)
    {
        string+= s + ", ";
    }
    string.truncate(string.length() - 2); //remove last ", "
    string+="}";
    return string;
}


FiniteAutomata FaToDFA::getDFA()
{
    FiniteAutomata FAd;
    FAd.alphabet = FA.alphabet;                                 //FAd alphabet

    QSet< QSet<QString> > new_states;
    QSet<QString> act_state;

    FAd.startState = "{" + FA.startState + "}";                 //FAd start state
    QSet<QString> tmp_set; tmp_set.insert(FA.startState);
    new_states.insert(tmp_set);
    do
    {

        act_state=*(new_states.begin());
        QString from = qsetToQstring(act_state);
        new_states.remove(act_state);
        FAd.states.insert(from);                                //insert new state to FAd
        foreach (QString a, FA.alphabet)
        {
            QSet<QString> discovered_state;
            foreach(ComputationalRules rule,FA.rules)
            {
                if(act_state.contains(rule.from)  && rule.symbol == a)
                {
                    discovered_state.insert(rule.to);
                }
            }


            QString to = qsetToQstring(discovered_state);
            if (!discovered_state.empty())
            {


                FAd.rules.insert(ComputationalRules(from,to,a));    //FAd insert rules
                if(!FAd.states.contains(to))
                    new_states.insert(discovered_state);
                //FAd.states.insert(to);
            }
        }
        //new finite states
        if(!act_state.intersect(FA.finalStates).empty())
        {
            FAd.finalStates.insert(from);                           //insert final states to FAd
        }
    }while (!new_states.empty());

    return FAd;
}

