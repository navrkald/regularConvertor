#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>
#include <QString>
#include "regexpparser.h"
#include "charpos.h"
#include "regexpnode.h"


class RegExp
{
public:
    explicit RegExp();
    explicit RegExp(QString _regexp, QObject *parent = 0);
    explicit RegExp(const RegExp& _regexp);

    bool Init(QString _strToVal);
    QList<CharPos> addConcOperator(QString _reqExp);


    RegExpParser m_parser;
    RegExpNode* m_rootNode;
    QString m_regExpStr;
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
    static bool IsAlphabetChar(QString symbol);
    static QString m_precedenceTable[7][7];
    void Clean();
};

QDataStream &operator<<(QDataStream &out, const RegExp &reg_exp);
QDataStream &operator>>(QDataStream &in, RegExp &reg_exp);
#endif // REGEXP_H
