#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
#include <qstandarditemmodel.h>
#include <QTimer>

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"
#define CHECK_STEP_TIMEOUT 1000

class CAlgorithm : public QStandardItemModel
{
    Q_OBJECT
public:
    CAlgorithm(QObject* parent = 0);
    enum modes {NONE, CHECK_MODE, PLAY_MODE, STEP_MODE};

    static const int hasBreakpointRole = Qt::UserRole;
    static const int breakpointRole = Qt::UserRole +1 ;
    static const int lastInstruction = -1 ;

    virtual void InitInstructions() = 0;
    virtual void RemoveFuture() = 0;
    void InitBreakpoints(int num);

protected:
    QVector <QString> m_instructions;
    QVector <bool> m_breakpoints;

    QColor m_actInstructionBackroundColor;
    QColor m_normalInstructionBackroundColor;

protected:
    int m_num;
    int m_instructionCount;
    int m_actInstruction;
    int m_prewInstruction;
    int m_actPos;
    QTimer *m_playTimer;
    QTimer *m_CheckStepTimer;
    modes m_mode;

    void SetActInstruction();
    void ClearActInstruction();

public slots:
    void SetMode(CAlgorithm::modes mode);
    void GetData(QModelIndex _index);
    void RunAlgorithm(int mil_sec);
    void Stop();
signals:
    void SendStatusBarMessage(QString message);
};
QDataStream& operator>>(QDataStream& in, CAlgorithm::modes& m);
#endif // ALGORITHM_H
