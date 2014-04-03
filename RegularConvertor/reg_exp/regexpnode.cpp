#include "regexpnode.h"

RegExpNode::RegExpNode(CharPos& _symbol) : processed(false)
{
    this->symbol = _symbol;
    this->str = _symbol.charter;
    parent = 0;
    this->state = UNKNOWN;
    this->selected = false;
}

void RegExpNode::copyTreeInOrder(const RegExpNode* orgTree, RegExpNode* copyTree)
{
    if(orgTree->children.count() != 0)
    {
        foreach(RegExpNode * old_child, orgTree->children)
        {
           RegExpNode * new_child = new RegExpNode(old_child->symbol);
           new_child->state = old_child->state;
           new_child->selected = old_child->selected;
           new_child->user_FA = old_child->user_FA;
           new_child->correct_FA = old_child->correct_FA;
           new_child->processed = old_child->processed;
           new_child->str = old_child->str;
           new_child->parent = copyTree;
           copyTree->children.append(new_child);
           copyTreeInOrder(old_child, new_child);
        }
    }
    else
    {
        RegExpNode* newNode  = copySingleNode(orgTree);
        newNode->parent = copyTree;
    }
}

RegExpNode * RegExpNode::copySingleNode(const RegExpNode* & orig_node)
{
    CharPos pos = orig_node->symbol;
    RegExpNode * copy_node = new RegExpNode(pos);
    copy_node->state = orig_node->state;
    copy_node->selected = orig_node->selected;
    copy_node->user_FA = orig_node->user_FA;
    copy_node->correct_FA = orig_node->correct_FA;
    copy_node->processed = orig_node->processed;
    copy_node->str = orig_node->str;
    return copy_node;
}

//RegExpNode::RegExpNode(RegExpNode*& reg_exp_node)
//{
//    this->symbol = reg_exp_node->symbol;
//    this->state = reg_exp_node->state;
//    this->selected = reg_exp_node->selected;
//    this->user_FA = reg_exp_node->user_FA;
//    this->correct_FA = reg_exp_node->correct_FA;
//    this->processed = reg_exp_node->processed;
//    this->str = reg_exp_node->str;
//    this->parent = 0;
//    copyTreeInOrder(reg_exp_node, this);
//}

RegExpNode::RegExpNode(RegExpNode *reg_exp_node)
{
    this->symbol = reg_exp_node->symbol;
    this->state = reg_exp_node->state;
    this->selected = reg_exp_node->selected;
    this->user_FA = reg_exp_node->user_FA;
    this->correct_FA = reg_exp_node->correct_FA;
    this->processed = reg_exp_node->processed;
    this->str = reg_exp_node->str;
    this->parent = 0;

    copyTreeInOrder(reg_exp_node, this);
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

bool RegExpNode::clearProcessed()
{
    clearProcessed(this);
}

void RegExpNode::clearProcessed(RegExpNode *node)
{
        foreach(RegExpNode* node1,node->children)
        {
            clearProcessed(node1);
        }
        if(node->str == "")
        {
            qDebug()<<"ERROR!!!!!";
        }
        node->processed = false;
}

bool RegExpNode::isLeaf()
{
    if(this->children.empty())
    {
        return true;
    }
    return false;
}
