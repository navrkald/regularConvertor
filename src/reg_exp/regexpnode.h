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
    RegExpNode(const CharPos& _symbolCharPos);
    RegExpNode();
//    RegExpNode(RegExpNode*& reg_exp_node);
    RegExpNode(RegExpNode* reg_exp_node);
    void copyTreeInOrder(const RegExpNode* orgTree, RegExpNode* copyTree);
    RegExpNode* copySingleNode(const RegExpNode*& copy_node);
    ~RegExpNode();
    bool childrenProcessed();
    void clearProcessed();
    static void clearProcessed(RegExpNode* node);
    bool isLeaf();
    static void save(RegExpNode* node, QDataStream& s);
    static RegExpNode* load(QDataStream& s);

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

QDataStream &operator<<(QDataStream& out, const RegExpNode* node);
QDataStream &operator>>(QDataStream& in, RegExpNode* node);

#endif // REGEXPNODE_H
