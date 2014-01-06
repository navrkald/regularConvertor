#include "regexpparser.h"

#define PLUS_INDEX              0
#define DOT_INDEX               1
#define STAR_INDEX              2
#define LEFT_BRACKET_INDEX      3
#define RIGHT_BRACKET_INDEX     4
#define SYMBOL_INDEX            5
#define DOLAR_INDEX             6

char RegExpParser::precedenceTable[7][7] =
{
  //       +    .    *    (    )    i    $   <--next symbol
/* + */  {'>', '<', '<', '<', '>', '<', '>'},
/* . */  {'>', '>', '<', '<', '>', '<', '>'},
/* * */  {'>', '>', '>', ' ', '>', '<', '>'},
/* ( */  {'<', '<', '<', '<', '=', '<', ' '},
/* ) */  {'>', '>', '>', ' ', '>', ' ', '>'},
/* i */  {'>', '>', '>', ' ', '>', ' ', '>'},
/* $ */  {'<', '<', '<', '<', ' ', '<', ' '}
// ^
// L : Top terminal on stack
};

QString RegExpParser::gramatic[5] =
{
    "E.E",
    "E+E",
    "E*",
    "i",
    "(E)"
};



RegExpParser::RegExpParser()
{
    rootNode = NULL;
}

bool RegExpParser::parse(QString text)
{
    //TODO
    // * list retezcu, jak se postupne bude obarvovat regularni vyraz
    // * strom
    // * instrukce
    //

    text.replace(" ", "");
    if(text == "")
    {
        return true;
    }

    inputString = addConcOperator(text);
    stack.init();
    nodeStack.clear();

    CharPos a = eatNextInputChar(); //first char on input
    CharPos b = stack.top();        //term on the top of stack

    do
    {
        switch(lookIntoTable(b.charter,a.charter))
        {
            case '=' :
                stack.push(a);
                a = eatNextInputChar();
                break;
            case '<' :
                stack.addHendler();
                stack.push(a);
                a = eatNextInputChar();
                break;
            case '>' :
                if(stack.reduce())
                {
                    int matchedRule = stack.getMatchedRule();
                    QList<CharPos> reducedString =  stack.getReducedString();
                    switch (matchedRule)
                    {
                        case SYMBOL_RULE:
                            if(reducedString.length() == 1)
                            {
                                CharPos symbol = reducedString.at(0);
                                RegExpNode* node = new RegExpNode(symbol);
                                nodeStack.push(node);
                            }
                            else
                            {
                                qDebug() << "Fatal error: Reduced string has lenth bigger then 1, when symbol rule matched!";
                                exit(EXIT_FAILURE);
                            }

                            break;
                        case CONC_RULE:
                            {
                                CharPos symbol = reducedString.at(1);
                                RegExpNode* node = new RegExpNode(symbol);
                                RegExpNode* rightSon =  nodeStack.pop();
                                RegExpNode* leftSon =  nodeStack.pop();
                                rightSon->parent = node;
                                leftSon->parent = node;
                                node->children.append(leftSon);
                                node->children.append(rightSon);
                                nodeStack.push(node);
                            }
                            break;
                        case OR_RULE:
                            {
                                CharPos symbol = reducedString.at(1);
                                RegExpNode* node = new RegExpNode(symbol);
                                RegExpNode* rightSon =  nodeStack.pop();
                                RegExpNode* leftSon =  nodeStack.pop();
                                rightSon->parent = node;
                                leftSon->parent = node;
                                node->children.append(leftSon);
                                node->children.append(rightSon);
                                nodeStack.push(node);
                            }
                            break;
                        case STAR_RULE:
                            {
                                CharPos symbol = reducedString.at(1);
                                RegExpNode* node = new RegExpNode(symbol);
                                RegExpNode* leftSon =  nodeStack.pop();
                                leftSon->parent = node;
                                node->children.append(leftSon);
                                nodeStack.push(node);
                            }
                            break;
                        case BRACKET_RULE:
                            break;
                        default:
                            qDebug() << "Fatal error: No rule found!";
                            exit(EXIT_FAILURE);
                            break;
                    }
                }
                else
                    return false;
                break;
            case ' ' :
                return false;
                break;
        }
        b = stack.top();        //term on the top of stack
    }while(!(a == dolar && b == dolar));
    if(!nodeStack.empty() && nodeStack.count() == 1)
    {
        rootNode = nodeStack.pop();
        rootNode->parent = 0;
    }
    else
    {
        qDebug() << "FATAL ERRPR: Node stack is empty or contains more then one root node!";
        exit(EXIT_FAILURE);
    }

    return true;
}

bool RegExpParser::isAlphabetChar(QString symbol)
{
    return symbol != "(" && symbol != ")" && symbol != "+" && symbol != "*" && symbol != "$" && symbol != ".";
}

CharPos RegExpParser::eatNextInputChar()
{
    CharPos firstString = inputString.at(0);
    inputString.removeAt(0);
    return firstString;
}

//doplni explicitne tecku jako konkatenaci tam kam patri
QList<CharPos> RegExpParser::addConcOperator(QString _reqExp)
{
    QList <CharPos> charList;
    for(int i = 0;i<_reqExp.length();i++)
    {
        CharPos charter(_reqExp.at(i),i,true);
        charList.append(charter);
    }
    for(int i = 0;i<charList.length()-1;i++)
    {
        QString j = charList.at(i).charter;
        QString k = charList.at(i+1).charter;
        if((isAlphabetChar(j) || j == ")" || j=="*") && (k=="(" || isAlphabetChar(k)))
        {
            CharPos charter(".",-1,true);
            charList.insert(i+1,charter);
            i++;
        }
    }
    CharPos dolar("$",-1,true);
    charList.insert(charList.length(),dolar);
    return charList;
}

int RegExpParser::getIndex(QString s)
{
    if(isAlphabetChar(s))
        return SYMBOL_INDEX;
    else if(s == PLUS)
        return PLUS_INDEX;
    else if(s == DOT)
        return DOT_INDEX;
    else if(s == STAR)
        return STAR_INDEX;
    else if(s == LEFT_BRACKET)
        return LEFT_BRACKET_INDEX;
    else if(s == RIGHT_BRACKET)
        return RIGHT_BRACKET_INDEX;
    else if(s == DOLAR)
        return DOLAR_INDEX;
    else
    {
        qDebug() << "FATAL EROOR: Can't find index of \"" << s << "\"!";
        exit (EXIT_FAILURE);
    }
}

char RegExpParser::lookIntoTable(QString a, QString b)
{
    return precedenceTable[getIndex(a)][getIndex(b)];
}
