#ifndef REGEXPTOFA_TEST_H
#define REGEXPTOFA_TEST_H

#include <QObject>
#include "../../RegularConvertor/algorithms/algorithm.h"
#include "../../RegularConvertor/reg_exp/regexp.h"

class RegExpToFA_test : public QObject
{
    Q_OBJECT
public:
    explicit RegExpToFA_test(QObject *parent = 0);
    
private:
    RegExp re;
    
private slots:
    void initTestCase();
    void computeSolution_test();
    void cleanupTestCase();
};

#endif // REGEXPTOFA_TEST_H
