#include "regexp.h"

//RegExp::precedenceTable =
//{

//}

//QString RegExp::strToVal;
//int RegExp::str_pos;

RegExp::RegExp()
{
    m_rootNode = new RegExpNode(emptyString);
}

RegExp::RegExp(QString _regexp,QObject*)
{
    m_regExpStr = _regexp;
    Init(m_regExpStr);
}

RegExp::RegExp(const RegExp &_regexp)
{
    this->m_rootNode = new RegExpNode(_regexp.m_rootNode);
    m_regExpStr = _regexp.m_regExpStr;
}

bool RegExp::Init(QString _strToVal)
{
    //init();
    m_regExpStr = _strToVal;
    m_regExpStr.replace(" ", "");
    if(m_regExpStr=="")
    {
        CharPos emptyString (EMPTYSET,0,true);
        m_rootNode = new RegExpNode(emptyString);
        return true;
    }
    if(m_parser.parse(m_regExpStr))
    {
        m_rootNode = m_parser.rootNode;
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


bool RegExp::IsAlphabetChar(QString symbol)
{
    return symbol != "(" && symbol != ")" && symbol != "+" && symbol != "*";
}

void RegExp::Clean()
{
    delete m_rootNode;
    m_rootNode = 0;
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
    out << reg_exp.m_regExpStr;
    RegExpNode::save(reg_exp.m_rootNode, out);
    return out;
}

QDataStream &operator>>(QDataStream &in, RegExp &reg_exp)
{
    in  >> reg_exp.m_regExpStr;
    reg_exp.m_rootNode =  RegExpNode::load(in);
    return in;
}
