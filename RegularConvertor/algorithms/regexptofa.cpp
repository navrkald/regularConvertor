#include "regexptofa.h"
#include <QtGlobal>

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"

#define HEADER 0
#define EMPTY_FA 1
#define EPSILON_FA 2
#define ONE_SYMBOL_FA 3
#define COMPOSED_FA 4
#define CONCATENATE_FA 5
#define ALTERNATE_FA 6
#define ITERATE_FA 7


RegExpToFA::RegExpToFA(RegExp* _re)
{
    setRE(_re);
}

RegExpToFA::RegExpToFA(AlgorithmWidget* _algorithm_widget, modes _mode, RegExpWidget *_re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget)
    : algorithm_widget(_algorithm_widget), mode(_mode), re_widget(_re_widget), left_fa_widget(_left_fa_widget), center_fa_widget(_center_fa_widget), right_fa_widget(_right_fa_widget)
{

    instructions.resize(ITERATE_FA+1);
    instructions[HEADER] = "<b>\"Zevnitř\" RV <i>r</i> opakovaně použít následující pravidla <br> ke konstrukci konečného automatu <i>M</i>:</b>";
    instructions[EMPTY_FA] = "Pro RV ∅ vytvoř KA <b><i>M<sub>∅</sub></i>:</b> ";
    instructions[EPSILON_FA] = QString("Pro RV %1 vytvoř FA <b><i>M<sub> %1 </sub></i>: </b>").arg(EPSILON);
    instructions[ONE_SYMBOL_FA] = "Pro RV <b>a</b> ∈ Σ vytvoř KA <b>M<sub>a</sub>: </b>";
    instructions[COMPOSED_FA] ="<b>Nechť</b> pro RV <b><i>r</i></b> a <b><i>t</i></b> již existují po řadě KA <b><i>M<sub>r</sub></i></b> a <b><i>M<sub>t</sub></i></b> <!--<br>--><b>Potom:</b>";
    instructions[CONCATENATE_FA] = INDENT "Pro RV <b>r.t</b> vytvoř KA <b><i>M<sub>r.t</sub></i>: </b>";
    instructions[ALTERNATE_FA] = INDENT "Pro RV <b>r+t</b> vytvoř KA <b><i>M<sub>r+t<sub></i>: </b>";
    instructions[ITERATE_FA] = INDENT "Pro RV <b>r*</b> vytvoř KA <b><i>M<sub>r*</sub></i>: </b>";

    QIcon empty_fa_icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");
    QIcon epsilon_fa_icon = QIcon(":/algorithms/algorithms/pictures/epsilon_fa.png");
    QIcon one_symbol_fa_icon = QIcon(":/algorithms/algorithms/pictures/one_symbol_fa.png");
    QIcon concatenate_fa_icon = QIcon(":/algorithms/algorithms/pictures/concatenate_fa.png");
    QIcon alternate_fa_icon = QIcon(":/algorithms/algorithms/pictures/alternate_fa.png");
    QIcon iterate_fa_icon = QIcon(":/algorithms/algorithms/pictures/iterate_fa.png");

    this->setColumnCount(1);
    this->setRowCount(instructions.count());

    for(int i = 0; i < instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,instructions[i],Qt::EditRole);
        setData(index,true,Algorithm::HasBrakepoint_Role);
        switch(i)
        {
            case HEADER:
                setData(index,false,Algorithm::HasBrakepoint_Role);
                break;
            case EMPTY_FA:
                setData(index,empty_fa_icon,Qt::DecorationRole);
                break;
            case EPSILON_FA:
                setData(index,epsilon_fa_icon,Qt::DecorationRole);
                break;

            case ONE_SYMBOL_FA:
                 setData(index,one_symbol_fa_icon,Qt::DecorationRole);
            break;
            case CONCATENATE_FA:
                setData(index,concatenate_fa_icon,Qt::DecorationRole);
            case ALTERNATE_FA:
                setData(index,alternate_fa_icon,Qt::DecorationRole);
                break;
            case ITERATE_FA:
                setData(index,iterate_fa_icon,Qt::DecorationRole);
                break;
        }
    }

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(nextStep()));

    connect(this->re_widget,SIGNAL(newRegExp(RegExp*)),this,SLOT(setRE(RegExp*))); //get RegExp when changed
    connect(this->algorithm_widget,SIGNAL(playPressed(int)),this,SLOT(runAlgorithm(int)));
    connect(this->algorithm_widget,SIGNAL(stopPressed()),this,SLOT(stop()));
    connect(this->algorithm_widget,SIGNAL(prewPressed()),this,SLOT(prewStep()));
    connect(this->algorithm_widget,SIGNAL(nextPressed()),this,SLOT(nextStep()));

}



void RegExpToFA::setRE(RegExp* _re)
{
    nodesToProcede.clear();
    this->re  = _re;
    //qDebug() << re->regexp;
    postOrder(re->rootNode);
    //computeSolution();
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
    }
}

void RegExpToFA::runAlgorithm(int mil_sec)
{
    timer->start(mil_sec);
}

void RegExpToFA::nextStep()
{
    if(!nodesToProcede.empty())
    {
        RegExpNode* processedNode = nodesToProcede.first();
        nodesToProcede.pop_front();

        if(processedNode->isLeaf())
        {
            processedNode->user_FA.init(processedNode->str);
        }
        else
        {
            if(processedNode->str == ITERATION)
            {
                 RegExpNode* son = processedNode->children.at(0);
                 processedNode->user_FA =  FiniteAutomata::iteration(son->user_FA);
            }
            else
            {
                RegExpNode* leftSon = processedNode->children.at(0);
                RegExpNode* rightSon = processedNode->children.at(1);
                if (processedNode->str == ALTERNATION)
                {
                     processedNode->user_FA = leftSon->user_FA + rightSon->user_FA;
                }
                else if (processedNode->str == CONCATENATION)
                {
                     processedNode->user_FA = FiniteAutomata::concatenate(leftSon->user_FA, rightSon->user_FA);
                }
            }
        }
        QList<RegExpNode*> children = processedNode->children;
        qDebug() << "Pocet synu: " << children.count();
        if(children.count() > 0)
        {
            left_fa_widget->setFA(&(children.at(0)->user_FA));
        }
        else
        {
            left_fa_widget->setFA(new FiniteAutomata());
        }
        if(children.count() > 1)
        {
            right_fa_widget->setFA(&(children.at(1)->user_FA));
        }
        else
        {
            right_fa_widget->setFA(new FiniteAutomata());
        }
        center_fa_widget->setFA(&(processedNode->user_FA));

        //set and update node icon
        processedNode->state = RegExpNode::CORRECT;
        QModelIndex index = re_widget->treeModel->indexFromNode(processedNode);
        re_widget->treeModel->dataChanged(index,index,QVector<int>(Qt::DecorationRole));
        re_widget->deselectAll();
        re_widget->treeView->selectionModel()->select(index,QItemSelectionModel::Select);
    }
    else
    {
        //qDebug() << "Nodes to precesed empty!";
    }


    //re_widget->selectionModel;
}

void RegExpToFA::prewStep()
{

}

void RegExpToFA::stop()
{
    timer->stop();
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

    foreach(RegExpNode* node1,node->children)
    {
        postOrder(node1);
    }
    //qDebug()<<"->" << node->str << "<-";
    if(node->str == "")
    {
        qDebug()<<"ERROR!!!!!";
    }
    nodesToProcede.append(node);
    foreach(RegExpNode* n,nodesToProcede)
    {
        qDebug()<< n->str;
    }
}

QList<RegExpNode*> RegExpToFA::getAvailableNodes()
{
    QList<RegExpNode*> availableNodes;
    QList<RegExpNode*> nodes_to_visit;
    nodes_to_visit.append(re->rootNode);
    while(!nodes_to_visit.empty())
    {
        RegExpNode* node = nodes_to_visit.first();
        nodes_to_visit.pop_front();

        if( !node->processed && node->childrenProcessed() )
        {
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
