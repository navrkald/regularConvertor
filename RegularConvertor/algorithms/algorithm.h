#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QVector>
#include <QString>
class algorithm
{
public:
    algorithm();
    QVector <QString> steps;
    struct steps
    {
        QString formal_text;
        QString informal_text;
        int pos;
    };
};

#endif // ALGORITHM_H
