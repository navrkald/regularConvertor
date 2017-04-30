#include "algorithm.h"
#include <algorithms/htmldelegate.h>

CAlgorithm::CAlgorithm(QObject* parent)
    : QStandardItemModel(parent), m_actInstructionBackroundColor(Qt::yellow), m_normalInstructionBackroundColor(Qt::white)
{
    m_playTimer = new QTimer();
    m_CheckStepTimer = new QTimer();
}

void CAlgorithm::InitBreakpoints(int num)
{
    m_breakpoints.resize(num);
    for(int i = 0; i < num; i++)
    {
        m_breakpoints[i] = false;
    }
}

void CAlgorithm::SetActInstruction()
{
    ClearActInstruction();
    QModelIndex index;
    if(m_actInstruction != lastInstruction)
    {
        index = this->index(m_actInstruction,0,QModelIndex());
    }
    else
    {
        index = this->index(m_prevInstruction,0,QModelIndex());
    }
    setData(index,QBrush(m_actInstructionBackroundColor),Qt::BackgroundRole);
}

//Clear act instruction from algorithm view
void CAlgorithm::ClearActInstruction()
{
    for(int i = 0; i<m_instructionCount; i++)
    {
        QModelIndex index = this->index(i,0,QModelIndex());
        setData(index,QBrush(m_normalInstructionBackroundColor),Qt::BackgroundRole);
    }
}

void CAlgorithm::SetupAlgorithmWidget(CAlgorithmWidget * algorithmWidget)
{
	HTMLDelegate* htmlDelegate = new HTMLDelegate();
	algorithmWidget->getAlgorithmView()->setModel(this);
	algorithmWidget->getAlgorithmView()->setItemDelegate(htmlDelegate);
	connect(htmlDelegate, SIGNAL(dataChanged(QModelIndex)), this, SLOT(getData(QModelIndex)));
}

QString CAlgorithm::JoinWithHtmlNewLines(QStringList variables)
{
	return variables.join("<br>");
}

void CAlgorithm::SetMode(AlgorithmModes mode)
{
  m_mode = mode;
}

void CAlgorithm::GetData(QModelIndex _index)
{
    m_breakpoints[_index.row()] = data(_index, CAlgorithm::breakpointRole).toBool();
}

void CAlgorithm::RunAlgorithm(int mil_sec)
{
    m_playTimer->start(mil_sec);
}

void CAlgorithm::Stop()
{
    m_playTimer->stop();
}

QDataStream& operator>>(QDataStream& in, AlgorithmModes& m)
{
    quint32 tmp;
    in >> tmp;
    m = (AlgorithmModes)tmp;
    return in;
}
