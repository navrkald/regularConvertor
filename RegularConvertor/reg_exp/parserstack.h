#ifndef PARSERSTACK_H
#define PARSERSTACK_H

#include "charpos.h"
#include "rules_defines.h"
#include <QDebug>

class ParserStack
{
public:
    ParserStack();
    void push(CharPos symbol);
    void addHendler();
    bool reduce();
    int getMatchedRule();
    QList<CharPos> getReducedString();
    bool empty();
    void init();
    CharPos top();
private:
    QList<CharPos> my_stack;
    QList<CharPos> reducedString;
    int findRule();
    bool isAlphabetChar(QString symbol);
    int matchedRule;
};

#endif // PARSERSTACK_H
