#include "regexp.h"

//RegExp::precedenceTable =
//{

//}

//QString RegExp::strToVal;
//int RegExp::str_pos;

RegExp::RegExp()
{
    CharPos emptyString (EMPTYSET,0,true);
    rootNode = new RegExpNode(emptyString);
}

RegExp::RegExp(QString _regexp,QObject*)
{
    regexp = _regexp;
    init(regexp);
}

RegExp::RegExp(const RegExp &_regexp)
{
    this->rootNode = new RegExpNode(_regexp.rootNode);
    regexp = _regexp.regexp;
}

bool RegExp::init(QString _strToVal)
{
    //init();
    regexp = _strToVal;
    regexp.replace(" ", "");
    if(regexp=="")
    {
        CharPos emptyString (EMPTYSET,0,true);
        rootNode = new RegExpNode(emptyString);
        return true;
    }
    if(parser.parse(regexp))
    {
        rootNode = parser.rootNode;
        return true;
    }
    else
        return false;
}

//bool RegExp::val_expression()
//{
//  if(!val_term())
//      return false;
//  if(!isControlChar() || matchToken("("))
//    return val_expression();
//  return true;
//}

//bool RegExp::val_term()
//{
//    if(!val_factor())
//        return false;
//    if(matchToken("*"))
//    {
//        movePos("*");
//        return false;
//    }
//    if(matchToken("+"))
//    {
//        movePos("+");
//        if(!val_term())
//            return false;
//    }
//    return true;
//}

//bool RegExp::val_factor()
//{
//    if(!isControlChar())
//    {
//        movePos(1);
//        return true;
//    }
//    else if(matchToken("("))
//    {
//        movePos("(");
//        val_expression();
//        if(matchToken(")"))
//        {
//            movePos(")");
//            return true;
//        }
//        else
//            return false;
//    }
//    else
//        return false;
//}

//bool RegExp::isControlChar()
//{
//    return matchToken("(") || matchToken(")") || matchToken("+") || matchToken("*");
//}

//bool RegExp::matchToken(const QString &str)
//{
//    return strToVal.mid(str_pos, str.length()) == str;
//}

//void RegExp::movePos(const QString& str)
//{
//    if (strToVal.mid(str_pos, str.length()) == str)
//    {
//        str_pos += str.length();
//    }
//}

//void RegExp::movePos(int lenth)
//{
//    str_pos += lenth;
//}


bool RegExp::isAlphabetChar(QString symbol)
{
    return symbol != "(" && symbol != ")" && symbol != "+" && symbol != "*";
}

void RegExp::clean()
{
    delete rootNode;
    rootNode = 0;
}

//doplni explicitne tecku jako konkatenaci tam kam patri
//QList<RegExp::CharPos> RegExp::addConcOperator(QString _reqExp)
//{
//    QList <RegExp::CharPos> charList;
//    for(int i = 0;i<_reqExp.length();i++)
//    {
//        RegExp::CharPos charter = {_reqExp.at(i),i};
//        charList.append(charter);
//    }
//    for(int i = 0;i<charList.length()-1;i++)
//    {
//        QString j = charList.at(i).charter;
//        QString k = charList.at(i+1).charter;
//        if((isAlphabetChar(j) || j == ")" || j=="*") && (k=="(" || isAlphabetChar(k)))
//        {
//            charList.insert(i+1,RegExp::CharPos{".",-1});
//            i++;
//        }
//    }
//    return charList;
//}


QDataStream &operator<<(QDataStream &out, const RegExp &reg_exp)
{
    out << reg_exp.regexp;
    return out;
}

QDataStream &operator>>(QDataStream &in, RegExp &reg_exp)
{
    in  >> reg_exp.regexp;
    return in;
}
