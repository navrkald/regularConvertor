#include "regexpnode.h"

RegExpNode::RegExpNode(CharPos& _symbol) : processed(false)
{
    this->symbol = _symbol;
    this->str = _symbol.charter;
    parent = 0;
    this->state = UNKNOWN;
}

RegExpNode::~RegExpNode()
{
    if(!children.empty())
        qDeleteAll(children);
}

//Pokud jsou vsechny deti spracovany nebo deti nema, vraci true jinak false.
bool RegExpNode::childrenProcessed()
{
    foreach(RegExpNode* node, this->children)
    {
        if(!node->processed)
        {
            return false;
        }
    }
    return true;
}

bool RegExpNode::isLeaf()
{
    if(this->children.empty())
    {
        return true;
    }
    return false;
}
