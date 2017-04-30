#include "htmlcreator.h"
#include <set/set_of_sets.h>

CHtmlCreator::CHtmlCreator()
{
}


CHtmlCreator::~CHtmlCreator()
{
}

QString CHtmlCreator::JoinWithHtmlNewLines(QStringList variables)
{
    return variables.join("<br>");
}

QString CHtmlCreator::PrintVariableValue(const QString &variable, const QString &value)
{
    return QString("<i><b>%1</i></b> = %2").arg(variable, value);
}

QString CHtmlCreator::PrintHtmlSet(QString variableName, QSet<QString> elements) {
    QString out = QString("<i><b>%1</b></i> = ").arg(variableName);
    out += qSetToQString(elements);
    return out;
}

QString CHtmlCreator::ConvertTextToHtml(const QString& nonHtmlText)
{
    return nonHtmlText.toHtmlEscaped();;
}
