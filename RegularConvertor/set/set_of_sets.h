#ifndef SET_OF_SETS_H
#define SET_OF_SETS_H
#include <QtCore>

uint qHash(const QSet<QString>);
QSet < QSet<QString> > findInSubsets(QSet < QSet<QString> > sets, QString s);
QSet < QSet<QString> > findInSubsets(QSet < QSet<QString> > sets, QSet<QString> strings);
QString qsetToQstring(QSet<QString> set);
QSet <QString> setOfSubsetsToSet(QSet < QSet<QString> > sets);

#endif // SET_OF_SETS_H
