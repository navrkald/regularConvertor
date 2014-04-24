#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
#include <qstandarditemmodel.h>
#include <QTimer>

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"
#define CHECK_STEP_TIMEOUT 1000

class Algorithm : public QStandardItemModel
{
    Q_OBJECT
public:
    Algorithm(QObject* parent = 0);
    //TODO add bool if brakepoint is set for some step
    QVector <QString> instructions;
    QVector <bool> breakpoints;
    enum modes {NONE, CHECK_MODE, PLAY_MODE, STEP_MODE};

    static const int HasBreakpoint_Role = Qt::UserRole;
    static const int Breakpoint_Role = Qt::UserRole +1 ;
    static const int last_instruction = -1 ;

    virtual void initInstructions() = 0;
    virtual void removeFuture() = 0;
    void initBreakpoints(int _num);


    QColor actInstructionBackroundColor;
    QColor normalInstructionBackroundColor;

    int num;
    int instruction_count;
    int actInstruction;
    int prewInstruction;
    int actPos;
    void setActInstruction();
    void clearActInstruction();
public slots:
    void setMode(Algorithm::modes _mode);
    void getData(QModelIndex _index);
    void runAlgorithm(int mil_sec);
    void stop();
signals:
    void sendStatusBarMessage(QString message);
protected:
    QTimer *play_timer;
    QTimer *check_step_timer;
};

#endif // ALGORITHM_H
