#ifndef HTML_CREATOR_H
#define HTML_CREATOR_H

#include <QString>
#include <QStringList>


class CHtmlCreator
{
public:
	CHtmlCreator();
    ~CHtmlCreator();
    static QString JoinWithHtmlNewLines(QStringList variables);
    static QString PrintVariableValue(const QString& variable, const QString& value);
    static QString PrintHtmlSet(const QString& variableName, const QSet<QString>& elements, const QString& separator = " ");
    static QString ConvertTextToHtml(const QString &nonHtmlText);
};

#endif // HTML_CREATOR_H
