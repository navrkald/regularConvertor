#include "algorithm.h"

Algorithm::Algorithm()
    : actInstructionBackroundColor(Qt::yellow), normalInstructionBackroundColor(Qt::white)
{
}

void Algorithm::initBreakpoints(int _num)
{
    breakpoints.resize(_num);
    foreach(bool breakpoint,breakpoints)
    {
        breakpoint = false;
    }
}

int Algorithm::setActInstruction()
{
    clearActInstruction();
    QModelIndex index = this->index(actInstruction,0,QModelIndex());
    setData(index,QBrush(actInstructionBackroundColor),Qt::BackgroundRole);
}

int Algorithm::clearActInstruction()
{
    for(int i = 0; i<instruction_count; i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,QBrush(normalInstructionBackroundColor),Qt::BackgroundRole);
    }
}
