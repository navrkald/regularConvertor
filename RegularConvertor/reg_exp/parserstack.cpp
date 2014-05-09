#include "parserstack.h"

ParserStack::ParserStack()
{
}

void ParserStack::push(CharPos symbol)
{
    my_stack.append(symbol);
}

void ParserStack::addHendler()
{
    for(int i =my_stack.length()-1;i>=0;i--)
    {
        if(my_stack.at(i).terminal)
        {
            CharPos left_angle_bracket("<", 1, false);
            my_stack.insert(i+1,left_angle_bracket);
            return;
        }
    }
}
bool ParserStack::reduce()
{
    bool hendlerFound = false;
    reducedString.clear();
    for(int i =my_stack.length()-1;i>=0;i--)
    {
        CharPos charter = my_stack.at(i);
        if(!charter.terminal && charter.charter == "<" && charter.pos > 0)
        {
            hendlerFound = true;
            my_stack.removeLast();
            break;
        }
        else
        {
            reducedString.insert(0,charter);
            my_stack.removeLast();
        }
    }

    if(!hendlerFound)
        return false;

    if((matchedRule = findRule()) == NO_RULE)
        return false;
    else
    {
        CharPos nonterminal("E",-1,false);
        push(nonterminal);
        return true;
    }

}

int ParserStack::getMatchedRule()
{
    return matchedRule;
}

bool ParserStack::empty()
{
    if(my_stack.length() == 1 )
    {
        CharPos first = my_stack.at(0);
        return first.terminal && first.pos == -1 && first.charter == "$";
    }
    else
        return false;
}

void ParserStack::init()
{
    my_stack.clear();
    CharPos dolar("$",-1,true);
    my_stack.insert(0,dolar);
}
//returns top TERMINAL
CharPos ParserStack::top()
{
    CharPos topTerminal;
    bool found = false;
    for(int i = my_stack.length()-1;i>=0;i--)
    {
        if(my_stack.at(i).terminal)
        {
            topTerminal = my_stack.at(i);
            found = true;
            break;
        }
    }
    if(!found)
    {
        qFatal("Fatal error: Not found any terminal in stack!");
        exit(-1);
    }
    return topTerminal;
}

int ParserStack::findRule()
{
    if(reducedString.length()>3)
        return NO_RULE;
    else if(reducedString.length()==1)
    {
        CharPos symbol = reducedString.at(0);
        if(symbol.terminal && symbol.pos >= 0 && isAlphabetChar(symbol.charter))
            return SYMBOL_RULE;
        else
            return NO_RULE;
    }
    else if(reducedString.length()==2)
    {
        CharPos first_symbol = reducedString.at(0);
        CharPos second_symbol = reducedString.at(1);
        if(!first_symbol.terminal && second_symbol.terminal && second_symbol.pos >= 0 && second_symbol.charter=="*")
            return STAR_RULE;
        else
            return NO_RULE;
    }
    else if(reducedString.length()==3)
    {
        CharPos first_symbol = reducedString.at(0);
        CharPos second_symbol = reducedString.at(1);
        CharPos third_symbol = reducedString.at(2);
        if(first_symbol.terminal && first_symbol.pos >= 0 && first_symbol.charter == "("
                && !second_symbol.terminal && third_symbol.terminal && third_symbol.pos >= 0 && third_symbol.charter == ")")
        {
            return BRACKET_RULE;
        }
        else if(!first_symbol.terminal && !third_symbol.terminal && second_symbol.terminal)
        {
            if(second_symbol.charter == "." && second_symbol.pos < 0)
                return CONC_RULE;
            else if (second_symbol.charter == "+" && second_symbol.pos >= 0)
                return OR_RULE;
            else
                return NO_RULE;
        }
        else
            return NO_RULE;
    }
    else
        return NO_RULE;
}

bool ParserStack::isAlphabetChar(QString symbol)
{
    return symbol != "(" && symbol != ")" && symbol != "+" && symbol != "*";
}


QList<CharPos> ParserStack::getReducedString()
{
    return reducedString;
}
