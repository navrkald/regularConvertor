#include "finiteautomata.h"
#include "algorithms/removeepsilon.h"
#include <htmlcreator.h>
#include <algorithms/constants.h>

#define NEW_COMMA_NAME

FiniteAutomata::FiniteAutomata()
{
    m_nextId = 0;
    m_startState = "";
}

//create finite automata from primitive reg expression
FiniteAutomata::FiniteAutomata(QString symbol)
{
    init(symbol);
}

FiniteAutomata::FiniteAutomata(const FiniteAutomata& _FA)
{
    this->m_nextId = _FA.m_nextId;
    m_states = _FA.m_states;
    m_alphabet = _FA.m_alphabet;
    m_rules = _FA.m_rules;
    m_startState = _FA.m_startState;
    m_finalStates = _FA.m_finalStates;
    m_coordinates = _FA.m_coordinates;
}

void FiniteAutomata::init(QString symbol)
{
    m_nextId = 0;

    //stavy
    QString newStartState =  this->createUniqueName();
    this->addState(newStartState);
    QString newEndState = this->createUniqueName();
    this->addState(newEndState);

    //symbol
    if(symbol != EPSILON)
    {
        this->addSymbol(symbol);
    }
    //pravidlo
    this->addRule(ComputationalRules(newStartState, newEndState, symbol));
    //startovni stav
    this->m_startState = newStartState;
    //koncovy stav
    this->addFinalState(newEndState);
}

QString FiniteAutomata::createUniqueName()
{
    //TODO overit zda v definici KA muze/nesmi byt abeceda a stavy disjunktni, pokud nesmi, pak udelat kontrolu zda novy unikatni stav nekoliduje s abecedou
    while(!isStateUnique(QString::number(m_nextId)))
       m_nextId++;
    return QString::number(m_nextId);
}

QList<ComputationalRules> FiniteAutomata::findRule_Symbol(QString symbol)
{
    QList <ComputationalRules> rules;
    foreach (ComputationalRules rule, this->m_rules)
    {
        if(rule.symbol == symbol)
            rules.append(rule);
    }
    return rules;
}

QList<ComputationalRules> FiniteAutomata::findRule_FromSymbol(QString from, QString symbol)
{
    QList <ComputationalRules> rules;
    foreach (ComputationalRules rule, this->m_rules)
    {
        if(rule.from == from && rule.symbol == symbol)
            rules.append(rule);
    }
    return rules;
}

QList<ComputationalRules> FiniteAutomata::findRule_FromSymbol(QSet<QString> from, QString symbol)
{
    QList <ComputationalRules> rules;
    foreach (ComputationalRules rule, this->m_rules)
    {
        foreach(QString f,from)
        {
            if(rule.from == f && rule.symbol == symbol)
                rules.append(rule);
        }
    }
    return rules;
}

QList<ComputationalRules> FiniteAutomata::findRule_From(QString from)
{
    QList <ComputationalRules> rules;
    foreach (ComputationalRules rule, this->m_rules)
    {
        if(rule.from == from)
            rules.append(rule);
    }
    return rules;
}

QList<QString> FiniteAutomata::findState_from(QString from)
{
    QList<QString> neighbours;
    foreach(ComputationalRules rule, this->m_rules)
    {
        if (rule.from == from)
            neighbours.append(rule.to);
    }
    return neighbours;
}

QList<QString> FiniteAutomata::findState_to(QString to)
{
    QList<QString> neighbours;
    foreach(ComputationalRules rule, this->m_rules)
    {
        if (rule.to == to)
            neighbours.append(rule.from);
    }
    return neighbours;
}

QList<QString> FiniteAutomata::getReachableStates()
{
    QString act_state;
    QList <QString> availableStates;
    QList <QString> visitedStates;
    availableStates.append(this->m_startState);
    do
    {
        act_state = availableStates.first();
        availableStates.pop_front();
        visitedStates.append(act_state);

        QList <QString> neighbours = findState_from(act_state);
        foreach (QString neighbour, neighbours)
        {
            if(!visitedStates.contains(neighbour))
               availableStates.append(neighbour);
        }
    }while (!availableStates.empty());
    return visitedStates;
}

QList<QString> FiniteAutomata::getReverseReachableStates()
{
    QString act_state;
    QList <QString> availableStates;
    QList <QString> visitedStates;
    availableStates.append(this->m_finalStates.toList());
    if(!availableStates.empty())
    {
        do
        {
            act_state = availableStates.first();
            availableStates.pop_front();
            visitedStates.append(act_state);

            QList <QString> neighbours = findState_to(act_state);
            foreach (QString neighbour, neighbours)
            {
                if(!visitedStates.contains(neighbour))
                   availableStates.append(neighbour);
            }
        }while (!availableStates.empty());
    }
    return visitedStates;
}

void FiniteAutomata::removeUnreachableStates()
{
    *this = this->removeUnreachableStates(*this);
}

void FiniteAutomata::removeNonTerminatingStates()
{
    *this = this->removeNonTerminatingStates(*this);
}

FiniteAutomata FiniteAutomata::makeWellDefined(FiniteAutomata FA)
{
    FA.toDFA();
    FiniteAutomata wellDefinedFA = FA;
    wellDefinedFA.removeUnreachableStates();
    wellDefinedFA.removeNonTerminatingStates();
    QString uniqName = wellDefinedFA.createUniqueName();
    foreach (QString state, wellDefinedFA.m_states)
    {
        foreach (QString symbol, wellDefinedFA.m_alphabet)
        {
             QList <ComputationalRules> rules = wellDefinedFA.findRule_From(state);
             bool searched = false;
             foreach(ComputationalRules rule, rules)
             {
                 if(rule.symbol == symbol)
                     searched = true;
             }
             if(!searched)
             {
                 wellDefinedFA.addState(uniqName);
                 wellDefinedFA.addRule(state,uniqName,symbol);
                 foreach(QString s,FA.m_alphabet)
                     wellDefinedFA.addRule(uniqName,uniqName,s);
             }
        }
    }
    return wellDefinedFA;
}

void FiniteAutomata::makeWellDefined()
{
    *this = this->makeWellDefined(*this);
}

FiniteAutomata FiniteAutomata::removeEpsilon(FiniteAutomata FA)
{
    if(!FA.hasEpsilon())
        return FA;

    FiniteAutomata FA_not_epsilon;
    FA_not_epsilon.m_states = FA.m_states;
    FA_not_epsilon.m_alphabet = FA.m_alphabet;
    FA_not_epsilon.m_startState = FA.m_startState;

    //Rules
    foreach(QString state,FA.m_states)
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
    foreach(QString state,FA.m_states)
    {
        QSet <QString> epsilon_closer = FA.epsilonCloser(state);
        foreach(QString state1, epsilon_closer)
        {
            if(FA.m_finalStates.contains(state1))
            {
                FA_not_epsilon.addFinalState(state);
            }
        }
    }
    return FA_not_epsilon;
}

void FiniteAutomata::removeEpsilon()
{
    *this = this->removeEpsilon(*this);
}

FiniteAutomata FiniteAutomata::toDFA(FiniteAutomata FA)
{
    if(FA.hasEpsilon())
    {
        FA.removeEpsilon();
    }

    if(FA.m_startState == "")
    {
        qWarning("WARNING: FA has no start state, returning original FA.");
        return FA;
    }

    FiniteAutomata FAd;
    FAd.m_alphabet = FA.m_alphabet;                                 //FAd alphabet

    QSet< QSet<QString> > new_states;
    QSet<QString> act_state;

    FAd.m_startState = "{" + FA.m_startState + "}";                 //FAd start state
    QSet<QString> tmp_set; tmp_set.insert(FA.m_startState);
    new_states.insert(tmp_set);
    do
    {
        act_state=*(new_states.begin());
        QString from = qSetToQString(act_state);
        new_states.remove(act_state);
        FAd.m_states.insert(from);                                //insert new state to FAd
        foreach (QString a, FA.m_alphabet)
        {
            QSet<QString> discovered_state;
            foreach(ComputationalRules rule,FA.m_rules)
            {
                if(act_state.contains(rule.from)  && rule.symbol == a)
                {
                    discovered_state.insert(rule.to);
                }
            }


            QString to = qSetToQString(discovered_state);
            if (!discovered_state.empty())
            {
                FAd.m_rules.insert(ComputationalRules(from,to,a));    //FAd insert rules
                if(!FAd.m_states.contains(to))
                    new_states.insert(discovered_state);
                //FAd.states.insert(to);
            }
        }
        //new finite states
        if(!act_state.intersect(FA.m_finalStates).empty())
        {
            FAd.m_finalStates.insert(from);                           //insert final states to FAd
        }
    }while (!new_states.empty());

    return FAd;
}

void FiniteAutomata::toDFA()
{
    *this = this->toDFA(*this);
}

FiniteAutomata FiniteAutomata::toMinFA(FiniteAutomata FA)
{
    if(FA.m_startState == "")
    {
        qWarning("WARNING: FA has no start state, returning original FA.");
        return FA;
    }
    if(FA.m_finalStates.empty())
    {
        qWarning("WARNING: FA has not any final states, returning original FA.");
        return FA;
    }

    FA.makeWellDefined();

    QSet< QSet <QString> > Qm;
    Qm.insert(FA.m_finalStates);
    if(FA.m_states != FA.m_finalStates)
            Qm.insert(FA.m_states - FA.m_finalStates);
    bool divided;
    QSet< QSet<QString> >::iterator X = Qm.begin();
    while (X != Qm.end())
    {
        divided = false;
        foreach(QString symbol,FA.m_alphabet)
        {
            QSet <QString> X1;
            QSet <QString> X2;
            if((canDivide(FA, symbol, Qm, *X, X1, X2)) == true)
            {// Do fision
                Qm.erase(X);
                Qm.insert(X1);
                Qm.insert(X2);
                divided = true;
                break;
            }
        }
        if(divided)
        {
            X = Qm.begin();
        }
        else
        {
            ++X;
        }
    }

    //Lets create new minFA
    FiniteAutomata minFA;
    minFA.m_alphabet = FA.m_alphabet;
    minFA.m_states = setOfSubsetsToSet(Qm);
    QSet<QString> gg =  *findInSubsets(Qm, FA.m_startState).begin();
    minFA.m_startState = qSetToQString(gg);
    minFA.m_finalStates = setOfSubsetsToSet(findInSubsets(Qm, FA.m_finalStates));
    //Add new rules
    foreach(QSet <QString> from_set,Qm)
    {
        foreach(QString symbol, FA.m_alphabet)
        {
            QList <ComputationalRules> rules =  FA.findRule_FromSymbol(from_set,symbol);
            QSet < QSet<QString> > froms = findInSubsets(Qm,getFroms(rules));
            QSet < QSet<QString> > tos = findInSubsets(Qm,getTos(rules));
            if(froms.count() != 1 || tos.count() != 1)
            {
                qFatal("Fatal Eroor: Fatal error in FiniteAutomata FiniteAutomata::toMinFA(FiniteAutomata FA)");
                exit(-1);
            }
            minFA.addRule(qSetToQString(*(froms.begin())), qSetToQString(*(tos.begin())),symbol);
        }
    }

    return minFA;
}

void FiniteAutomata::toMinFA()
{
    *this = this->toMinFA(*this);
}

FiniteAutomata FiniteAutomata::normalize(FiniteAutomata FA)
{
    if(FA.m_startState == "")
    {
        qWarning("WARNING: FA has no start state, returning original FA.");
        return FA;
    }

    //inspirated by http://iris.uhk.cz/tein/teorie/normalizace.html
    //Check if is minimal FA FiniteAutomata minFa;
    FiniteAutomata normalizedFA = FA;


    //renaming states for sure that
    //intersect new naming and old names will be empty set
    foreach(QString state, FA.m_states)
    {
        normalizedFA.RenameState(state,state+"'");
    }

    QList<QString> sorted_alphabet = FA.m_alphabet.toList();
    qSort(sorted_alphabet);

    QList <QString> renamed_nodes;
    QList <QString> processed_nodes;
    int counter = 0;
    normalizedFA.RenameState(normalizedFA.m_startState,QString::number(counter));
    counter++;
    renamed_nodes.append(normalizedFA.m_startState);
    do
    {
        QString q = normalize_chooseSmallestNonprocessed(renamed_nodes,processed_nodes);
        foreach(QString symbol,sorted_alphabet)
        {
            QList<ComputationalRules> rules = normalizedFA.findRule_FromSymbol(q,symbol);
            if(rules.count() == 1)
            {
                ComputationalRules rule = rules.first();
                if (! renamed_nodes.contains(rule.to))
                {
                    normalizedFA.RenameState(rule.to,QString::number(counter));
                    renamed_nodes.append(QString::number(counter));
                    counter++;
                }
            }
            else if(rules.count() > 1)
            {
                qFatal("Fatal Error: Trying to normalize not determinized FA!");
                exit(-1);
            }
        }
        processed_nodes.append(q);
    }while(processed_nodes != renamed_nodes);
    return normalizedFA;
}


FiniteAutomata FiniteAutomata::removeUnreachableStates(FiniteAutomata FA)
{
    FiniteAutomata clean_FA = FA;
    QSet<QString> reachableStates = FA.getReachableStates().toSet();
    QSet<QString> unReachableStates = FA.m_states - reachableStates;
    clean_FA.removeStates(unReachableStates);
    return clean_FA;
}

FiniteAutomata FiniteAutomata::removeNonTerminatingStates(FiniteAutomata FA)
{
    FiniteAutomata clean_FA = FA;
    if(clean_FA.m_finalStates.count() > 0)
    {
        QSet <QString> terminating_states = FA.getReverseReachableStates().toSet();
        QSet <QString> non_terminating_states = FA.m_states - terminating_states;
        clean_FA.removeStates(non_terminating_states);
    }
    return clean_FA;
}

FiniteAutomata FiniteAutomata::concatenate(FiniteAutomata FA1, FiniteAutomata FA2)
{
    FiniteAutomata FA;

    //stavy
    FiniteAutomata FA2_uniq_states = FA2;

    FA.m_states = FA1.m_states + FA2.m_states;

    //prejmenovani stavu
    QSet <QString> same_states = FA1.m_states & FA2.m_states; //prunik názvů stavů
    if(same_states.size() != 0)
    {
        foreach (QString str, same_states)
        {

#ifdef NEW_NUM_NAME
            QString uniqName = FA.createUniqueName();
#endif

#ifdef NEW_COMMA_NAME
            QString uniqName = str;
            do
            {
                uniqName += "'";
            }
            while(!FA.isStateUnique(uniqName));
#endif

            FA2_uniq_states.RenameState(str,uniqName);
            FA.addState(uniqName);
        }
    }



    // abeceda
    FA.m_alphabet = FA1.m_alphabet + FA2.m_alphabet;

    //koncove stavy
    FA.m_finalStates = FA2_uniq_states.m_finalStates;

    //Pocatecni stav
    FA.m_startState = FA1.m_startState;

    //pravidla
    FA.m_rules = FA1.m_rules + FA2_uniq_states.m_rules;
    foreach(QString end_state,FA1.m_finalStates)
    {
        ComputationalRules bridge(end_state, FA2_uniq_states.m_startState, EPSILON);
        FA.addRule(bridge);
    }

    return FA;
}

FiniteAutomata FiniteAutomata::iteration(FiniteAutomata FA1)
{
    FiniteAutomata FA;

    //stavy
    FA.m_states = FA1.m_states;
    QString newStartState = FA.createUniqueName();
    FA.m_states.insert(newStartState);
    QString newEndState = FA.createUniqueName();
    FA.m_states.insert(newEndState);

    // abeceda
    FA.m_alphabet = FA1.m_alphabet;

    //koncove stavy
    FA.m_finalStates.insert(newEndState);

    //Pocatecni stav
    FA.m_startState = newStartState;

    //pravidla
    FA.m_rules = FA1.m_rules ;
    FA.addRule(ComputationalRules(newStartState,FA1.m_startState,EPSILON)); //startovni stav
    foreach(QString end_state ,FA1.m_finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON)); //koncove stavy do noveho
        FA.addRule(ComputationalRules(end_state,FA1.m_startState,EPSILON)); //z puvodniho koncoveho stavu do puvodniho startovniho
    }
    FA.addRule(ComputationalRules(newStartState,newEndState,EPSILON)); //ze startovniho stavu do koncoveho


    return FA;
}

bool FiniteAutomata::AreEquivalent(FiniteAutomata FA1, FiniteAutomata FA2)
{
    FiniteAutomata minFA1 = FA1;
    FiniteAutomata minFA2 = FA2;

    minFA1.removeEpsilon();
    minFA1.toDFA();
    minFA1.toMinFA();
    minFA1 = minFA1.normalize(minFA1);

    minFA2.removeEpsilon();
    minFA2.toDFA();
    minFA2.toMinFA();
    minFA2 = minFA2.normalize(minFA2);

    return minFA1 == minFA2;
}

bool FiniteAutomata::isStateUnique(QString state)
{
    return !m_states.contains(state);
}

//prida stav
//pokud je jmeno unikatni vrati true, jinak false
bool FiniteAutomata::addState(QString stateName)
{
    if(!isStateUnique(stateName))
        return false;

    m_states.insert(stateName);
    return true;

}


bool FiniteAutomata::removeState(QString stateName)
{
    bool retval = this->m_states.remove(stateName);
    removeFinalState(stateName);
    if(this->m_startState == stateName)
    {
        this->m_startState = "";
    }
    foreach(ComputationalRules rule,this->m_rules)
    {
        if (rule.from == stateName || rule.to == stateName)
            removeRule(rule);
    }
    return retval;
}

void FiniteAutomata::removeStates(QSet<QString> states)
{
    foreach (QString state, states)
    {
        removeState(state);
    }
}

bool FiniteAutomata::RenameState(QString oldStateName, QString newStateName)
{
    if(!this->isStateUnique(newStateName))
        return false;

    //rename start state
    if(m_startState == oldStateName)
        m_startState = newStateName;

    // Rename coordinates
    m_coordinates[newStateName] = m_coordinates[oldStateName];
    m_coordinates.remove(oldStateName);

    //rename state in finalStates
    if(m_finalStates.contains(oldStateName))
    {
        this->removeFinalState(oldStateName);
        this->addFinalState(newStateName);
    }

    //rename state name in all rules
    foreach(ComputationalRules rule,m_rules)
    {
        if(rule.from == oldStateName || rule.to == oldStateName)
        {
            ComputationalRules newRule = rule;
            if(rule.from == oldStateName)
                newRule.from = newStateName;
            if(rule.to == oldStateName)
                newRule.to = newStateName;
            this->addRule(newRule);
            this->removeRule(rule);
        }
    }
    //rename state in states set
    this->removeState(oldStateName);
    return addState(newStateName);
}

void FiniteAutomata::SetStartState(QString StateName)
{
    this->m_startState = StateName;
}

QString FiniteAutomata::GetStartState()
{
    return m_startState;
}

void FiniteAutomata::addFinalState(QString StateName)
{
    m_finalStates.insert(StateName);
}

void FiniteAutomata::removeFinalState(QString StateName)
{
    m_finalStates.remove(StateName);
}

void FiniteAutomata::addSymbol(QString symbol)
{
    m_alphabet.insert(symbol);
}

void FiniteAutomata::removeSymbol(QString symbol)
{
    m_alphabet.remove(symbol);
    QList<ComputationalRules> rules_to_dell = findRule_Symbol(symbol);
    foreach(ComputationalRules rule, rules_to_dell)
    {
        m_rules.remove(rule);
    }
}

QList<QString> FiniteAutomata::get_sorted_states()
{
    QList<QString> states_list = m_states.toList();
    qSort(states_list);
    return states_list;
}

QVector<QString> FiniteAutomata::GetSortedAlphabet()
{
    QVector<QString> alphabetVector = m_states.toList().toVector();
    qSort(alphabetVector);
    return alphabetVector;

}

bool FiniteAutomata::addRule(ComputationalRules rule)
{
    if(m_rules.contains(rule))
    {//Mnozina pravidel uz obsahuje "<<rule.from<<" " <<rule.symbol<<"->"<<rule.to
        return false;
    }
    m_rules.insert(rule);
    if(!m_alphabet.contains(rule.symbol) && rule.symbol != EPSILON)
        m_alphabet.insert(rule.symbol);
    return true;
}

bool FiniteAutomata::addRule(QString from, QString to, QString symbol)
{
    ComputationalRules newRule(from,to,symbol);
    return addRule(newRule);
}

bool FiniteAutomata::removeRule(ComputationalRules rule)
{
		QString symbol = rule.symbol;
		bool changed = m_rules.remove(rule);
    if(findRule_Symbol(symbol).count() == 0)
        m_alphabet.remove(symbol);
	return changed;
}

bool FiniteAutomata::changeSymbolInRule(ComputationalRules rule, QString symbol)
{
    removeRule(rule);
    ComputationalRules newRule = rule;
    newRule.symbol = symbol;
    return addRule(newRule);
}


//vrati true kdyz jsou odlisne, jinak true
bool FiniteAutomata::changeRule(ComputationalRules oldrule, ComputationalRules newrule)
{
    if(oldrule == newrule)
    {
        return false;
    }
    else
    {
        m_rules.remove(oldrule);
        m_rules.insert(newrule);
        return true;
    }
}

void FiniteAutomata::SetAplhabet(QSet<QString> alphabet)
{
    m_alphabet = alphabet;
	m_alphabet.remove(EPSILON);
}

void FiniteAutomata::AddAplhabet(QSet<QString> alphabet)
{
	m_alphabet += alphabet;
	m_alphabet.remove(EPSILON);
}

bool FiniteAutomata::hasEpsilon()
{
    foreach(ComputationalRules rule,m_rules)
    {
        if(rule.symbol == EPSILON)
        {
            return true;
        }
    }
    return false;
}

bool FiniteAutomata::isDeterministic()
{
    foreach (ComputationalRules rule, this->m_rules)
    {
        if(findRule_FromSymbol(rule.from,rule.symbol).count() > 1)
            return false;
    }
    return true;
}

QSet<QString> FiniteAutomata::epsilonCloser(QString state)
{
    //empty set
    QSet<QString> Q_act;
    if (!this->m_states.contains(state))
    {
        return Q_act;
    }

    //search epsilon closer
    Q_act.insert(state);
    QSet<QString> Q_prew;
    do
    {
        Q_prew = Q_act;
        foreach (QString state1, Q_act)
            Q_act += epsilonNeighbours(state1);
    }while(Q_act != Q_prew);
    return Q_act;
}

void FiniteAutomata::SetStates(const QSet<QString> &states)
{
    m_states = states;
}

void FiniteAutomata::SetFinalStates(const QSet<QString> &finalStates)
{
    m_finalStates = finalStates;
}

QSet<QString> FiniteAutomata::epsilonNeighbours(QString state)
{
    QSet<QString> epsilon_neighbours;
    if (!this->m_states.contains(state))
    {
        return epsilon_neighbours;
    }
    else
    {
        foreach(ComputationalRules rule,this->m_rules)
        {
            if(rule.from == state && rule.symbol == EPSILON)
                epsilon_neighbours.insert(rule.to);
        }
        return epsilon_neighbours;
    }

}

QSet<ComputationalRules> FiniteAutomata::nonEpsilonRulesOf(QString state)
{
    QSet<ComputationalRules> non_epsilon_rules;
    foreach(ComputationalRules rule, this->m_rules)
    {
        if(rule.from == state && rule.symbol != EPSILON)
        {
            non_epsilon_rules.insert(rule);
        }
    }
    return non_epsilon_rules;
}

QString FiniteAutomata::normalize_chooseSmallestNonprocessed(QList <QString> renamed, QList <QString> processed)
{
    QString smallest;
    qSort(renamed);
    foreach(QString q ,renamed)
    {
        if(!processed.contains(q))
        {
            smallest = q;
            break;
        }
    }
    return smallest;
}

QString FiniteAutomata::PrintHtmlStates()
{
    return CHtmlCreator::PrintHtmlSet("Q", m_states);
}

QString FiniteAutomata::PrintHtmlAlphabet()
{
    return CHtmlCreator::PrintHtmlSet(SIGMA, m_alphabet);
}

QString FiniteAutomata::PrintHtmlRules()
{
    return CHtmlCreator::PrintHtmlSet("R", ComputationalRulesToQSetOfStrings(m_rules));
}

QString FiniteAutomata::PrintHtmlStartState()
{
	return QString("<i><b>start state</b></i> := %1").arg(m_startState);
}

QString FiniteAutomata::PrintHtmlFinalStates()
{
    return CHtmlCreator::PrintHtmlSet("F", m_finalStates);
}

//helper function for method toMinFA()
bool FiniteAutomata::canDivide(FiniteAutomata FA ,QString symbol, QSet<QSet<QString> > Qm, QSet<QString> X, QSet<QString> &X1, QSet<QString> &X2)
{
    QSet <QString> Q1;
    QList<ComputationalRules> rules;

    //add all rules with from = X and witch symbol
    foreach(QString from,X)
    {
        rules.append(FA.findRule_FromSymbol(from,symbol));
    }

    //come throw all rules
    foreach (ComputationalRules rule,rules)
    {
        if(!X1.empty())
        {
            if (Q1 == *findInSubsets(Qm,rule.to).begin())
            {
                X1.insert(rule.from);
            }
            else
            {
                X2.insert(rule.from);
            }
        }
        else //for first create new Q1
        {
            QSet < QSet<QString> > tmp = findInSubsets(Qm,rule.to);
            if(tmp.empty())
            {
                qFatal("Fatal error: in function bool FiniteAutomata::canDivide(FiniteAutomata FA ,QString symbol, QSet<QSet<QString> > Qm, QSet<QString> X, QSet<QString> &X1, QSet<QString> &X2)");
                exit(-1);
            }
            Q1 = *findInSubsets(Qm,rule.to).begin();
            X1.insert(rule.from);
        }
    }

    if (X2.empty())
        return false;
    else
        return true;
}


FiniteAutomata operator +(const FiniteAutomata FA1, const FiniteAutomata FA2)
{
    FiniteAutomata FA;

    //stavy
    FA.m_states = FA1.m_states + FA2.m_states;

    //prejmenovani stavu
    FiniteAutomata FA2_uniq_states = FA2;
    QSet <QString> same_states = FA1.m_states & FA2.m_states; //prunik názvů stavů
    if(same_states.size() != 0)
    {
        foreach (QString str, same_states)
        {
#ifdef NEW_NUM_NAME
            QString uniqName = FA.createUniqueName();
#endif

#ifdef NEW_COMMA_NAME
            QString uniqName = str;
            do
            {
                uniqName += "'";
            }
            while(!FA.isStateUnique(uniqName));
#endif

            FA2_uniq_states.RenameState(str,uniqName);
            FA.addState(uniqName);
        }
    }


    QString newStartState = FA.createUniqueName();
    FA.m_states.insert(newStartState);
    QString newEndState = FA.createUniqueName();
    FA.m_states.insert(newEndState);



    // abeceda
    FA.m_alphabet = FA1.m_alphabet + FA2_uniq_states.m_alphabet;

    //koncove stavy
    FA.m_finalStates.insert(newEndState);

    //Pocatecni stav
    FA.m_startState = newStartState;

    //pravidla
    FA.m_rules = FA1.m_rules + FA2_uniq_states.m_rules;
    FA.addRule(ComputationalRules(newStartState,FA1.m_startState,EPSILON));
    FA.addRule(ComputationalRules(newStartState,FA2_uniq_states.m_startState,EPSILON));
    foreach(QString end_state ,FA1.m_finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }
    foreach(QString end_state, FA2_uniq_states.m_finalStates)
    {
        FA.addRule(ComputationalRules(end_state, newEndState, EPSILON));
    }

    return FA;

}


bool operator ==(const FiniteAutomata FA1, const FiniteAutomata FA2)
{
    if (FA1.m_states != FA2.m_states)
        return false;

    if (FA1.m_alphabet != FA2.m_alphabet)
        return false;

    if (FA1.m_startState != FA2.m_startState)
        return false;

    if (FA1.m_finalStates != FA2.m_finalStates)
        return false;

    if (FA1.m_rules != FA2.m_rules)
        return false;

    return true;
}

QDebug operator<<(QDebug d, const FiniteAutomata &FA)
{
   d << "FA=(Q,S,R,s,F)";
   d << "Q={" << FA.m_states <<"}";
   d << "S={" << FA.m_alphabet << "}";
   d << "R={" << FA.m_rules << "}";
   d << "s= "<< FA.m_startState;
   d << "F=" << FA.m_finalStates << "}";
   d << FA.m_coordinates;
   return d;
}

QDataStream& FiniteAutomata::operator<<(QDataStream &out)
{
    out << m_states << m_startState << m_finalStates << m_alphabet  << m_rules << (quint32)m_nextId << m_coordinates;
    return out;
}



QDataStream& FiniteAutomata::operator>>(QDataStream &in)
{
    quint32 nextId;
    in  >> m_states >> m_startState >> m_finalStates >> m_alphabet  >> m_rules >> nextId >> m_coordinates;
    m_nextId = (int)nextId;
    return in;
}
