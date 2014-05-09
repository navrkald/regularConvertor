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


RegExpToFA::RegExpToFA(RegExp* _re, modes _mode) : Algorithm()
{
    setMode(_mode);
    setRE(_re);
}

RegExpToFA::RegExpToFA(AlgorithmWidget* _algorithm_widget, modes _mode, RegExpWidget *_re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget, RegExp* _re, QObject* parrent)
    : Algorithm(parrent), algorithm_widget(_algorithm_widget), mode(_mode), re_widget(_re_widget), left_fa_widget(_left_fa_widget), center_fa_widget(_center_fa_widget), right_fa_widget(_right_fa_widget)
{
    actInstruction = HEADER;
    re = 0;
    instruction_count = ITERATE_FA+1;

    initInstructions();
    initBreakpoints(instruction_count);

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
        setData(index,true,Algorithm::HasBreakpoint_Role);
        setData(index,false,Algorithm::Breakpoint_Role);
        switch(i)
        {
            case HEADER:
            case COMPOSED_FA:
                setData(index,false,Algorithm::HasBreakpoint_Role);
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
    setMode(mode);

    //
    // Connect algorithm buttons.
    //
    connect(this->algorithm_widget,SIGNAL(playPressed(int)),this,SLOT(runAlgorithm(int)));
    connect(this->algorithm_widget,SIGNAL(stopPressed()),this,SLOT(stop()));
    connect(this->algorithm_widget,SIGNAL(prewPressed()),this,SLOT(prevStep()));
    connect(this->algorithm_widget,SIGNAL(nextPressed()),this,SLOT(nextStep()));
    connect(this->algorithm_widget, SIGNAL(checkSolutionPressed()), this, SLOT(checkSolution()));
    connect(this->algorithm_widget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(showCorrectSolution()));
    connect(this->algorithm_widget, SIGNAL(showUserSolutionPressed()), this, SLOT(showUserSolution()));

    //
    // Connect timers.
    //
    connect(play_timer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(check_step_timer, SIGNAL(timeout()), this, SLOT(checkSolution()));

    //
    // Connect regexp widget
    //
    connect(this->re_widget,SIGNAL(newRegExp(RegExp*)),this,SLOT(setRE(RegExp*))); //get RegExp when changed
    connect(this->re_widget,SIGNAL(itemClicked(QModelIndex)),this,SLOT(selectRegExp(QModelIndex)));

    re_widget->setRegExp(_re);
    re_widget->modelChanged();

    // because not implemented
    algorithm_widget->disableShowButton();
}

void RegExpToFA::setMode(modes _mode)
{
    mode = _mode;
    nodesToProcede.clear();
    if(mode==STEP_MODE)
    {
        check_step_timer->start(CHECK_STEP_TIMEOUT);
    }
    else if(mode == NONE)
    {
        ;
    }
    else
    {
        check_step_timer->stop();
    }

    if(re != 0)
    {
        history.clear();
        num = 0;
        clearActInstruction();
        //checking modes
        if(mode == PLAY_MODE)
        {
            //this clears regexp tree, this by the way indirectly calls void RegExpToFA::setRE(RegExp *_re)
            //this will cause to call setRE and there saveStep() function is called
            re_widget->setRegExp(new RegExp(re->regexp));
            re_widget->modelChanged();

            nodesToProcede.clear();
            postOrder(re->rootNode);
        }
        else if(mode == CHECK_MODE || mode == STEP_MODE)
        {
            this->re->rootNode->clearProcessed();
            computeSolution();
        }
    }

}

void RegExpToFA::setRE(RegExp *_re)
{
    //Has impact to computeSolution() and nextStep()
    nodesToProcede.clear();

    //clean automatas and act instruction in algorithm view
    if(mode != NONE)
    {
        left_fa_widget->setFA(new FiniteAutomata());
        right_fa_widget->setFA(new FiniteAutomata());
        center_fa_widget->setFA(new FiniteAutomata());

        QModelIndex top_intex = re_widget->treeModel->index(0,0,QModelIndex());
        //ui->treeView->selectionCommand(top_intex,new QEvent(QEvent::MouseButtonPress));
        re_widget->treeView->selectionModel()->select(top_intex,QItemSelectionModel::Select);
        emit re_widget->treeView->clicked(top_intex);
    }


    //set RE in algorithm
    this->re  = _re;


    //checking modes
    if(mode == PLAY_MODE)
    {
        //unselect instruction from algorithm window
        clearActInstruction();

        //this is not prew step so we have to save it in history
        if(history.empty() || re->regexp !=  history.last().re->regexp)
        {
            actPos = 0;
            actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
            history.clear();
            saveStep();
        }
        //aby se naplnily nodestoprocese po tom co jsme se vrátili v hystorii
        postOrder(re->rootNode);
    }
    else if(mode == CHECK_MODE || mode == STEP_MODE)
    {
        computeSolution();
        postOrder(re->rootNode);
    }
}

void RegExpToFA::selectRegExp(QModelIndex index)
{

    RegExpNode* node = re_widget->treeModel->nodeFromIndex(index);
    QList<RegExpNode*> children = node->children;
    if(children.count() > 0)
    {
        left_fa_widget->setFA(&children.at(0)->user_FA);
    }
    else
    {
        left_fa_widget->setFA(new FiniteAutomata());
    }
    if(children.count() > 1)
    {
        right_fa_widget->setFA(&children.at(1)->user_FA);
    }
    else
    {
        right_fa_widget->setFA(new FiniteAutomata());
    }
        center_fa_widget->setFA(&node->user_FA);

    re_widget->treeModel->dataChanged(index,index,QVector<int>(Qt::DecorationRole));
    re_widget->deselectAll();
    re_widget->treeView->selectionModel()->select(index,QItemSelectionModel::Select);
}

void RegExpToFA::saveStep()
{
    if(re->rootNode->str == "")
        qFatal("Fatal Error: in funcion void RegExpToFA::saveStep()");
    steps s;
    s.re = new RegExp(*re);
    s.num = ++num;
    s.actInstruction = actInstruction;
    history.append(s);
    actPos = history.count() - 1;
}

void RegExpToFA::computeSolution()
{
    nodesToProcede.clear();
    re->rootNode->clearProcessed();
    postOrder(re->rootNode);
    if(nodesToProcede.count() == 1 && nodesToProcede.first()->str == EMPTYSET)
    {
        RegExpNode* processedNode = nodesToProcede.first();
        processedNode->correct_FA = FiniteAutomata();
        processedNode->correct_FA.states << "0";
        processedNode->correct_FA.startState = "0";
    }
    else
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
}

//void RegExpToFA::runAlgorithm(int mil_sec)
//{
//    play_timer->start(mil_sec);
//}

void RegExpToFA::nextStep()
{
    //re->rootNode->clearProcessed();
    if(!nodesToProcede.empty())
    {
        prewInstruction = actInstruction;


        RegExpNode* processedNode = nodesToProcede.first();
        nodesToProcede.pop_front();

        if(processedNode->isLeaf())
        {
            if(history.count() == 1 && nodesToProcede.empty() && processedNode->str == EMPTYSET)
            {
                processedNode->user_FA = FiniteAutomata();
                processedNode->user_FA.states << "0";
                processedNode->user_FA.startState = "0";
                actInstruction = EMPTY_FA;
            }
            else
            {
                processedNode->user_FA.init(processedNode->str);
                if(processedNode->str == EPSILON)
                {
                    actInstruction = EPSILON_FA;
                }
                else
                {
                    actInstruction = ONE_SYMBOL_FA;
                }
            }
        }
        else
        {
            if(processedNode->str == ITERATION)
            {
                 RegExpNode* son = processedNode->children.at(0);
                 processedNode->user_FA =  FiniteAutomata::iteration(son->user_FA);

                 actInstruction = ITERATE_FA;
            }
            else
            {
                RegExpNode* leftSon = processedNode->children.at(0);
                RegExpNode* rightSon = processedNode->children.at(1);
                if (processedNode->str == ALTERNATION)
                {
                    processedNode->user_FA = leftSon->user_FA + rightSon->user_FA;

                    actInstruction = ALTERNATE_FA;
                }
                else if (processedNode->str == CONCATENATION)
                {
                     processedNode->user_FA = FiniteAutomata::concatenate(leftSon->user_FA, rightSon->user_FA);

                     actInstruction = CONCATENATE_FA;
                }
            }
        }

        if(breakpoints[actInstruction])
            play_timer->stop();

        setActInstruction();

        //set and update node icon
        processedNode->state = RegExpNode::CORRECT;
        processedNode->processed = true;
        QModelIndex index = re_widget->treeModel->indexFromNode(processedNode);
        selectRegExp(index);

        removeFuture();
        saveStep();
    }
    else
    {
        play_timer->stop();
    }
}

void RegExpToFA::prevStep()
{
    if (actPos > 0)
    {
        actPos--;
        num = history.at(actPos).num;
        actInstruction = history.at(actPos).actInstruction;
        RegExp* tmp_re = history.at(actPos).re;
        re_widget->setRegExp(new RegExp(*tmp_re));

        re_widget->modelChanged();
        setActInstruction();
    }
}

void RegExpToFA::removeFuture()
{
    int count  = history.count();
    for(int i = actPos+1; i < count; i++)
    {
        history.removeLast();
    }
}


//void RegExpToFA::stop()
//{
//    play_timer->stop();
//}

//void RegExpToFA::getData(QModelIndex _index)
//{
//    breakpoints[_index.row()] = data(_index, Algorithm::Breakpoint_Role).toBool();
//}

void RegExpToFA::checkSolution()
{
    nodesToProcede.clear();
    postOrder(re->rootNode);
    QList<RegExpNode*> nodes_to_check(nodesToProcede);
    while(!nodes_to_check.empty())
    {
        RegExpNode* node = nodes_to_check.first();
        nodes_to_check.pop_front();
        if(FiniteAutomata::areEquivalent(node->correct_FA, node->user_FA))
        {
            node->state = RegExpNode::CORRECT;
        }
        else if(FiniteAutomata::areEquivalent(FiniteAutomata(), node->user_FA))
        {
            node->state = RegExpNode::UNKNOWN;
        }
        else
        {
            node->state = RegExpNode::WRONG;
        }
        QModelIndex index = re_widget->treeModel->indexFromNode(node);
        re_widget->treeModel->dataChanged(index,index,QVector<int>(Qt::DecorationRole));
    }
}

void RegExpToFA::showCorrectSolution()
{

}

void RegExpToFA::showUserSolution()
{

}

void RegExpToFA::setExample(RegExp *_re)
{
    re_widget->setRegExp(_re);
    re_widget->modelChanged();
    setMode(CHECK_MODE);
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
    if(node->str == "")
    {
        qFatal("Fatal error:(this shoud not ever happen) in function void RegExpToFA::postOrder(RegExpNode* node)");
        exit(-1);
    }
    if(! node->processed) //because of restore point in hystori
        nodesToProcede.append(node);
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



void RegExpToFA::initInstructions()
{

    instructions.resize(instruction_count);
    instructions[HEADER] = "<b>\"Zevnitř\" RV <i>r</i> opakovaně použít následující pravidla <br> ke konstrukci konečného automatu <i>M</i>:</b>";
    instructions[EMPTY_FA] = "Pro RV ∅ vytvoř KA <b><i>M<sub>∅</sub></i>:</b> ";
    instructions[EPSILON_FA] = QString("Pro RV %1 vytvoř FA <b><i>M<sub> %1 </sub></i>: </b>").arg(EPSILON);
    instructions[ONE_SYMBOL_FA] = "Pro RV <b>a</b> ∈ Σ vytvoř KA <b>M<sub>a</sub>: </b>";
    instructions[COMPOSED_FA] ="<b>Nechť</b> pro RV <b><i>r</i></b> a <b><i>t</i></b> již existují po řadě KA <b><i>M<sub>r</sub></i></b> a <b><i>M<sub>t</sub></i></b> <!--<br>--><b>Potom:</b>";
    instructions[CONCATENATE_FA] = INDENT "Pro RV <b>r.t</b> vytvoř KA <b><i>M<sub>r.t</sub></i>: </b>";
    instructions[ALTERNATE_FA] = INDENT "Pro RV <b>r+t</b> vytvoř KA <b><i>M<sub>r+t<sub></i>: </b>";
    instructions[ITERATE_FA] = INDENT "Pro RV <b>r*</b> vytvoř KA <b><i>M<sub>r*</sub></i>: </b>";
}
