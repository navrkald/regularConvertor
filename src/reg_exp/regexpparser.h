#ifndef REGEXPPARSER_H
#define REGEXPPARSER_H

#include <QString>
#include <QList>
#include "charpos.h"
#include "rules_defines.h"
#include "parserstack.h"
#include <QDebug>
#include <QStack>
#include "regexpnode.h"

#define PLUS            "+"
#define DOT             "."
#define STAR            "*"
#define LEFT_BRACKET    "("
#define RIGHT_BRACKET   ")"
#define DOLAR           "$"

#define PLUS_INDEX              0
#define DOT_INDEX               1
#define STAR_INDEX              2
#define LEFT_BRACKET_INDEX      3
#define RIGHT_BRACKET_INDEX     4
#define SYMBOL_INDEX            5
#define DOLAR_INDEX             6

class RegExpParser
{

public:
    RegExpParser();
    static char precedenceTable[7][7];
    static QString gramatic[5];
    bool parse(QString text);
    static bool isAlphabetChar(QString symbol);
    RegExpNode* rootNode;

private:
    QList<CharPos> inputString;
    ParserStack stack;
    CharPos eatNextInputChar();
    QList<CharPos> addConcOperator(QString _reqExp);
    int getIndex(QString s);
    char lookIntoTable(QString a,QString b);
    QStack <RegExpNode*> nodeStack;

};

#endif // REGEXPPARSER_H
