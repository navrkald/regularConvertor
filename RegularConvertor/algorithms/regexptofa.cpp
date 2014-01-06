#include "regexptofa.h"
#include <QtGlobal>

RegExpToFA::RegExpToFA(RegExp _re)
{
    this->re  = _re;
}

void RegExpToFA::computeSolution()
{
    QList<RegExpNode*> availableNodes;
    while((availableNodes = getAvailableNodes()).count() != 0)
    {
       RegExpNode* processedNode = chooseNode();
       processedNode->processed = true;
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



RegExpNode* RegExpToFA::chooseNode()
{
   QList<RegExpNode*> availableNodes = getAvailableNodes();
   int count = availableNodes.count();
   int random = qrand () % count;
   return availableNodes.at(random);
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
            availableNodes.append(node);
        }
        else
        {
            nodes_to_visit.append(node->children);
        }
    }
    return availableNodes;
}
