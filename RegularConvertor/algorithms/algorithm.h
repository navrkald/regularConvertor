#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
#include <qstandarditemmodel.h>

class Algorithm : public QStandardItemModel
{
    Q_OBJECT
public:
    Algorithm();
    //TODO add bool if brakepoint is set for some step
    QVector <QString> instructions;
    enum modes {CHECK_MODE, PLAY_MODE, STEP_MODE};

    static const int HasBrakepoint_Role = Qt::UserRole;
    static const int Brakepoint_Role = Qt::UserRole +1 ;

    //    struct steps
//    {
//        QString formal_text;
//        QString informal_text;
//        int pos;
//    };
};

#endif // ALGORITHM_H
