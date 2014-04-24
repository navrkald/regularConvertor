#include "algorithm.h"

Algorithm::Algorithm(QObject* parent)
    : QStandardItemModel(parent), actInstructionBackroundColor(Qt::yellow), normalInstructionBackroundColor(Qt::white)
{
    play_timer = new QTimer();
    check_step_timer = new QTimer();
}

void Algorithm::initBreakpoints(int _num)
{
    breakpoints.resize(_num);
    foreach(bool breakpoint,breakpoints)
    {
        breakpoint = false;
    }
}

void Algorithm::setActInstruction()
{
    clearActInstruction();
    QModelIndex index;
    if(actInstruction != last_instruction)
    {
        index = this->index(actInstruction,0,QModelIndex());
    }
    else
    {
        index = this->index(prewInstruction,0,QModelIndex());
    }
    setData(index,QBrush(actInstructionBackroundColor),Qt::BackgroundRole);
}

//Clear act instruction from algorithm view
void Algorithm::clearActInstruction()
{
    for(int i = 0; i<instruction_count; i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,QBrush(normalInstructionBackroundColor),Qt::BackgroundRole);
    }
}

void Algorithm::setMode(Algorithm::modes)
{

}

void Algorithm::getData(QModelIndex _index)
{
    breakpoints[_index.row()] = data(_index, Algorithm::Breakpoint_Role).toBool();
}

void Algorithm::runAlgorithm(int mil_sec)
{
    play_timer->start(mil_sec);
}

void Algorithm::stop()
{
    play_timer->stop();
}
