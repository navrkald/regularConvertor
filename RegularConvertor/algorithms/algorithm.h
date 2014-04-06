#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
#include <qstandarditemmodel.h>

#define INDENT "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"

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

    virtual void initInstructions() = 0;
    void initBreakpoints(int _num);


    QColor actInstructionBackroundColor;
    QColor normalInstructionBackroundColor;

    int instruction_count;
    int actInstruction;
    int prewInstruction;
    int setActInstruction();
    int clearActInstruction();
public slots:
    void getData(QModelIndex _index);
//signals:
//    instructionChanged(int _instruction);
    //    struct steps
//    {
//        QString formal_text;
//        QString informal_text;
//        int pos;
//    };
};

#endif // ALGORITHM_H
