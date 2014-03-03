#include "regexptofa.h"
#include <QtGlobal>

#define EMPTY_FA 1

RegExpToFA::RegExpToFA(RegExp _re)
{
    this->re  = _re;
    postOrder(re.rootNode);

}

RegExpToFA::RegExpToFA(modes mode, RegExpWidget *re_widget, FA_widget* left_fa_widget, FA_widget* center_fa_widget, FA_widget* right_fa_widget)
{
    this->setRowCount(10);
    this->setColumnCount(1);
    instructions.resize(100);
    instructions[0] = "\"Zevnitř\" RV <i>r</i> opakovaně použít následující pravidla ke konstrukci konečného automatu <i>M</i>:";
    instructions[1] = "Pro RV ∅ vytvoř KA <i>M<sub>∅</sub></i>";
    QIcon empty_fa_icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");

    for(int i = 0; i < instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,instructions[i],Qt::EditRole);
        if(i == EMPTY_FA)
        {
            setData(index,empty_fa_icon,Qt::DecorationRole);
        }
    }



}

void RegExpToFA::computeSolution()
{
    while(nodesToProcede.count() != 0)
    {
       RegExpNode* processedNode = nodesToProcede.first();//chooseRandomNode();
       nodesToProcede.pop_front();

       if(processedNode->isLeaf())
       {
           processedNode->correct_FA.init(processedNode->str);
       }
       else
       {
           if(processedNode->str == ITERATION)
           {
                RegExpNode* son = processedNode->children.at(0);
                processedNode->correct_FA =  FiniteAutomata::iteration(son->correct_FA);
           }
           else
           {
               RegExpNode* leftSon = processedNode->children.at(0);
               RegExpNode* rightSon = processedNode->children.at(1);
               if (processedNode->str == ALTERNATION)
               {
                    processedNode->correct_FA = leftSon->correct_FA + rightSon->correct_FA;
               }
               else if (processedNode->str == CONCATENATION)
               {
                    processedNode->correct_FA = FiniteAutomata::concatenate(leftSon->correct_FA, rightSon->correct_FA);
               }
           }
       }
    } //while

}



RegExpNode* RegExpToFA::chooseRandomNode()
{
   QList<RegExpNode*> availableNodes = getAvailableNodes();
   int count = availableNodes.count();
   int random = qrand () % count;
   return availableNodes.at(random);
}

void RegExpToFA::postOrder(RegExpNode* node)
{
    //if (node->children.count() != 0)
    //{
        foreach(RegExpNode* node1,node->children)
        {
            postOrder(node1);
        }
    //}
    nodesToProcede.append(node);
}

QList<RegExpNode*> RegExpToFA::getAvailableNodes()
{
    QList<RegExpNode*> availableNodes;
    QList<RegExpNode*> nodes_to_visit;
    nodes_to_visit.append(re.rootNode);
    while(!nodes_to_visit.empty())
    {
        RegExpNode* node = nodes_to_visit.first();
        nodes_to_visit.pop_front();

        if( !node->processed && node->childrenProcessed() )
        {
            //availableNodes.append(node);
            node->processed = true;
            availableNodes.append(node);
        }
        else
        {
            nodes_to_visit.append(node->children);
        }
    }
    return availableNodes;
}
