#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
#include <qstandarditemmodel.h>
class Algorithm : public QStandardItemModel
{
public:
    Algorithm();
    //TODO add bool if brakepoint is set for some step
    QVector <QString> instructions;
    enum modes {CHECK_MODE, PLAY_MODE, STEP_MODE};
//    struct steps
//    {
//        QString formal_text;
//        QString informal_text;
//        int pos;
//    };
};

#endif // ALGORITHM_H
