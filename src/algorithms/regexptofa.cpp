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


RegExpToFA::RegExpToFA(RegExp* _re, modes _mode) : CAlgorithm()
{
    SetMode(_mode);
    setRE(_re);
}

RegExpToFA::RegExpToFA(CAlgorithmWidget* _algorithm_widget, modes _mode, RegExpWidget *_re_widget, FA_widget* _left_fa_widget, FA_widget* _center_fa_widget, FA_widget* _right_fa_widget, RegExp* _re, QObject* parrent)
    : CAlgorithm(parrent), algorithm_widget(_algorithm_widget), mode(_mode), re_widget(_re_widget), left_fa_widget(_left_fa_widget), center_fa_widget(_center_fa_widget), right_fa_widget(_right_fa_widget)
{
    m_actInstruction = HEADER;
    re = 0;
    m_instructionCount = ITERATE_FA+1;

    InitInstructions();
    InitBreakpoints(m_instructionCount);

    QIcon empty_fa_icon = QIcon(":/algorithms/algorithms/pictures/empty_fa.png");
    QIcon epsilon_fa_icon = QIcon(":/algorithms/algorithms/pictures/epsilon_fa.png");
    QIcon one_symbol_fa_icon = QIcon(":/algorithms/algorithms/pictures/one_symbol_fa.png");
    QIcon concatenate_fa_icon = QIcon(":/algorithms/algorithms/pictures/concatenate_fa.png");
    QIcon alternate_fa_icon = QIcon(":/algorithms/algorithms/pictures/alternate_fa.png");
    QIcon iterate_fa_icon = QIcon(":/algorithms/algorithms/pictures/iterate_fa.png");

    this->setColumnCount(1);
    this->setRowCount(m_instructions.count());

    for(int i = 0; i < m_instructions.count();i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,m_instructions[i],Qt::EditRole);
        setData(index,true,CAlgorithm::hasBreakpointRole);
        setData(index,false,CAlgorithm::breakpointRole);
        switch(i)
        {
            case HEADER:
            case COMPOSED_FA:
                setData(index,false,CAlgorithm::hasBreakpointRole);
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
    SetMode(mode);

    //
    // Connect algorithm buttons.
    //
    connect(this->algorithm_widget,SIGNAL(playPressed(int)),this,SLOT(RunAlgorithm(int)));
    connect(this->algorithm_widget,SIGNAL(stopPressed()),this,SLOT(Stop()));
    connect(this->algorithm_widget,SIGNAL(prewPressed()),this,SLOT(prevStep()));
    connect(this->algorithm_widget,SIGNAL(nextPressed()),this,SLOT(nextStep()));
    connect(this->algorithm_widget, SIGNAL(checkSolutionPressed()), this, SLOT(checkSolution()));
    connect(this->algorithm_widget, SIGNAL(showCorrectSolutionPressed()), this, SLOT(showCorrectSolution()));
    connect(this->algorithm_widget, SIGNAL(showUserSolutionPressed()), this, SLOT(showUserSolution()));
    connect(this->algorithm_widget, SIGNAL(beginPressed()), this, SLOT(toBegin()));
    connect(this->algorithm_widget, SIGNAL(endPressed()), this, SLOT(toEnd()));

    //
    // Connect timers.
    //
    connect(m_playTimer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(m_CheckStepTimer, SIGNAL(timeout()), this, SLOT(checkSolution()));

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

void RegExpToFA::SetMode(modes _mode)
{
    mode = _mode;
    nodesToProcede.clear();
    if(mode==STEP_MODE)
    {
        m_CheckStepTimer->start(CHECK_STEP_TIMEOUT);
    }
    else if(mode == NONE)
    {
        ;
    }
    else
    {
        m_CheckStepTimer->stop();
    }

    if(re != 0)
    {
        history.clear();
        m_num = 0;
        ClearActInstruction();
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
        ClearActInstruction();

        //this is not prew step so we have to save it in history
        if(history.empty() || re->regexp !=  history.last().re->regexp)
        {
            m_actPos = 0;
            m_actInstruction = HEADER; //init start instruction because new regExp may appeare when pres step mode was in run
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
    s.num = ++m_num;
    s.actInstruction = m_actInstruction;
    history.append(s);
    m_actPos = history.count() - 1;
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
        m_prewInstruction = m_actInstruction;


        RegExpNode* processedNode = nodesToProcede.first();
        nodesToProcede.pop_front();

        if(processedNode->isLeaf())
        {
            if(history.count() == 1 && nodesToProcede.empty() && processedNode->str == EMPTYSET)
            {
                processedNode->user_FA = FiniteAutomata();
                processedNode->user_FA.states << "0";
                processedNode->user_FA.startState = "0";
                m_actInstruction = EMPTY_FA;
            }
            else
            {
                processedNode->user_FA.init(processedNode->str);
                if(processedNode->str == EPSILON)
                {
                    m_actInstruction = EPSILON_FA;
                }
                else
                {
                    m_actInstruction = ONE_SYMBOL_FA;
                }
            }
        }
        else
        {
            if(processedNode->str == ITERATION)
            {
                 RegExpNode* son = processedNode->children.at(0);
                 processedNode->user_FA =  FiniteAutomata::iteration(son->user_FA);

                 m_actInstruction = ITERATE_FA;
            }
            else
            {
                RegExpNode* leftSon = processedNode->children.at(0);
                RegExpNode* rightSon = processedNode->children.at(1);
                if (processedNode->str == ALTERNATION)
                {
                    processedNode->user_FA = leftSon->user_FA + rightSon->user_FA;

                    m_actInstruction = ALTERNATE_FA;
                }
                else if (processedNode->str == CONCATENATION)
                {
                     processedNode->user_FA = FiniteAutomata::concatenate(leftSon->user_FA, rightSon->user_FA);

                     m_actInstruction = CONCATENATE_FA;
                }
            }
        }

        if(m_breakpoints[m_actInstruction])
            m_playTimer->stop();

        SetActInstruction();

        //set and update node icon
        processedNode->state = RegExpNode::CORRECT;
        processedNode->processed = true;
        QModelIndex index = re_widget->treeModel->indexFromNode(processedNode);
        selectRegExp(index);

        RemoveFuture();
        saveStep();
    }
    else
    {
        m_playTimer->stop();
    }
}

void RegExpToFA::prevStep()
{
    if (m_actPos > 0)
    {
        m_actPos--;
        m_num = history.at(m_actPos).num;
        m_actInstruction = history.at(m_actPos).actInstruction;
        RegExp* tmp_re = history.at(m_actPos).re;
        re_widget->setRegExp(new RegExp(*tmp_re));

        re_widget->modelChanged();
        SetActInstruction();
    }
}

void RegExpToFA::RemoveFuture()
{
    int count  = history.count();
    for(int i = m_actPos+1; i < count; i++)
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
    SetMode(CHECK_MODE);
}

void RegExpToFA::toBegin()
{
    m_actPos=0;
    m_num = history.at(m_actPos).num;
    m_actInstruction = history.at(m_actPos).actInstruction;
    RegExp* tmp_re = history.at(m_actPos).re;
    re_widget->setRegExp(new RegExp(*tmp_re));

    re_widget->modelChanged();
    SetActInstruction();
}

void RegExpToFA::toEnd()
{
    while(!nodesToProcede.empty())
    {
        nextStep();
    }
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



void RegExpToFA::InitInstructions()
{

    m_instructions.resize(m_instructionCount);
    m_instructions[HEADER] = tr("<b>\"From inside\" RegExp <i>r</i> repeatedly use this rules <br>for construction of the finite automata<i>M</i>:</b>");
    m_instructions[EMPTY_FA] = tr("For RegExp ∅ create FA <b><i>M<sub>∅</sub></i>:</b> ");
    m_instructions[EPSILON_FA] = tr("For RegExp " EPSILON " create FA <b><i>M<sub>" EPSILON "</sub></i>: </b>");
    m_instructions[ONE_SYMBOL_FA] = tr("For RegExp <b>a</b> ∈ Σ create FA <b>M<sub>a</sub>: </b>");
    m_instructions[COMPOSED_FA] = tr("<b>Let</b> for RegExp <b><i>r</i></b> a <b><i>t</i></b>  exists FA <b><i>M<sub>r</sub></i></b> a <b><i>M<sub>t</sub></i></b> <!--<br>--><b>Then:</b>");
    m_instructions[CONCATENATE_FA] = INDENT + tr("For RegExp <b>r.t</b> create FA <b><i>M<sub>r.t</sub></i>: </b>");
    m_instructions[ALTERNATE_FA] = INDENT + tr("For RegExp <b>r+t</b> create FA <b><i>M<sub>r+t<sub></i>: </b>");
    m_instructions[ITERATE_FA] = INDENT + tr("For RegExp <b>r*</b> create FA <b><i>M<sub>r*</sub></i>: </b>");
}
