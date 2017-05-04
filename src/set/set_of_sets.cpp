#include <set/set_of_sets.h>
#include <algorithms/constants.h>

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

QString qSetToQString(const QSet<QString>& set, const QString& separator)
{
	if (set.empty())
        return EMPTYSET;

	QList <QString> list = set.toList();
	qSort(list);
	//reverse list
	//for(int k = 0; k < (list.size()/2); k++) list.swap(k,list.size()-(1+k));

	QString string = "{" + separator;
	QStringList string_list = list;
	string += string_list.join("," + separator);
	string += separator + "}";
	return string;
}

QString qVectorToQString(QVector<QString> elements)
{
    qSort(elements);
    QString string = "{";
    QStringList string_list = elements.toList();
    string+= string_list.join(", ");
    string+="}";
    return string;
}


QSet <QString> setOfSubsetsToSet(QSet < QSet<QString> > sets)
{
    QSet <QString> newSet;
    foreach(QSet<QString> set, sets)
    {
        newSet.insert(qSetToQString(set));
    }
    return newSet;
}
