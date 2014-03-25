#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>
#include <QString>
#include "regexpparser.h"
#include "charpos.h"
#include "regexpnode.h"

#define EMPTY_SET

class RegExp
{
public:
    explicit RegExp();
    explicit RegExp(QString _regexp, QObject *parent = 0);
    explicit RegExp(const RegExp& _regexp);

    bool init(QString _strToVal);
    QList<CharPos> addConcOperator(QString _reqExp);
    RegExpParser parser;
    RegExpNode* rootNode;
    QString regexp;
private:


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
    void clean();

};
#endif // REGEXP_H
