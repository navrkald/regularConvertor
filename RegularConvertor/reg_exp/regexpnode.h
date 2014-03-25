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
//    RegExpNode(RegExpNode*& reg_exp_node);
    RegExpNode(RegExpNode* reg_exp_node);
    void copyTreeInOrder(const RegExpNode* orgTree, RegExpNode* copyTree);
    RegExpNode* copySingleNode(const RegExpNode*& copy_node);
    ~RegExpNode();
    bool childrenProcessed();
    bool isLeaf();
    enum states {CORRECT,WRONG,UNKNOWN};
    states state;
    bool selected;
    FiniteAutomata user_FA;
    FiniteAutomata correct_FA;
    bool processed;
    CharPos symbol;
    QString str;
    RegExpNode *parent;
    QList<RegExpNode *> children;
};

#endif // REGEXPNODE_H
