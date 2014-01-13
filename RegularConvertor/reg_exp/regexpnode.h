#ifndef REGEXPNODE_H
#define REGEXPNODE_H

#include <QList>
#include <QString>
#include "charpos.h"
#include "../finite_machine/finiteautomata.h"

#define CONCATENATION "."
#define ALTERNATION "+"
#define ITERATION "*"

//Tato trida reprezntuje RV jako srom strom

class RegExpNode
{
public:
    RegExpNode(CharPos& _symbol);
    ~RegExpNode();
    bool childrenProcessed();
    bool isLeaf();

    FiniteAutomata user_FA;
    FiniteAutomata correct_FA;
    bool processed;
    CharPos symbol;
    QString str;
    RegExpNode *parent;
    QList<RegExpNode *> children;
};

#endif // REGEXPNODE_H
