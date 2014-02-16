#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
class Algorithm
{
public:
    Algorithm();
    //TODO add bool if brakepoint is set for some step
    QVector <QString> steps;
    struct steps
    {
        QString formal_text;
        QString informal_text;
        int pos;
    };
};

#endif // ALGORITHM_H
