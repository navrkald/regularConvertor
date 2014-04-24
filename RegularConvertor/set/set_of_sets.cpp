#include "set/set_of_sets.h"

uint qHash(const QSet<QString>) {
  uint seed = 0;

//  for(auto x : c) {
//    seed ^= qHash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//  }

  return seed;
}

QSet < QSet<QString> > findInSubsets(QSet < QSet<QString> > sets, QString s)
{
    QSet < QSet<QString> > result;
    foreach(QSet <QString> set,sets)
    {
        if (set.contains(s))
        {
            result.insert(set);
        }
    }
    return result;
}

QSet < QSet<QString> > findInSubsets(QSet < QSet<QString> > sets, QSet<QString> strings)
{
    QSet < QSet<QString> > result;
    foreach(QSet <QString> set,sets)
    {
        foreach(QString s,strings)
        {
            if (set.contains(s))
            {
                result.insert(set);
            }
        }
    }
    return result;
}

QString qsetToQstring(QSet<QString> set)
{
    if (set.empty())
        return "∅";

    QList <QString> list = set.toList();
    qSort(list);
    //reverse list
    //for(int k = 0; k < (list.size()/2); k++) list.swap(k,list.size()-(1+k));

    QString string = "{";
    QStringList string_list = list;
    string+= string_list.join(",");
    string+="}";
    return string;
}


QSet <QString> setOfSubsetsToSet(QSet < QSet<QString> > sets)
{
    QSet <QString> newSet;
    foreach(QSet<QString> set, sets)
    {
        newSet.insert(qsetToQstring(set));
    }
    return newSet;
}
