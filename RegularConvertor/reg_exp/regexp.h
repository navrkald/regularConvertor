#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>
#include <QString>
#include "reg_exp/regexpparser.h"
#include "reg_exp/charpos.h"
#include "reg_exp/regexpnode.h"

class RegExp
{
public:
    explicit RegExp();
    explicit RegExp(QString _regexp, QObject *parent = 0);
    bool parse(QString _strToVal);
    QList<CharPos> addConcOperator(QString _reqExp);
    RegExpParser parser;
    RegExpNode* rootNode;

private:
    QString regexp;

    //functions to validate reqexp
//    static QString strToVal;
//    static int str_pos;
//    static bool matchToken(const QString &str);
//    static bool val_expression();
//    static bool val_term();
//    static bool val_factor();
//    static bool isControlChar();
//    static void movePos(const QString& str);
//    static void movePos(int lenth);
    static bool isAlphabetChar(QString symbol);
    static QString precedenceTable[7][7];
    void init();

};
#endif // REGEXP_H
