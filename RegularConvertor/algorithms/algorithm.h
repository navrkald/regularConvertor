#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
class Algorithm
{
public:
    Algorithm();
    QVector <QString> steps;
    struct steps
    {
        QString formal_text;
        QString informal_text;
        int pos;
    };
};

#endif // ALGORITHM_H
