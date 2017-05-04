#include "htmlcreator.h"
#include <set/set_of_sets.h>
#include <algorithms/constants.h>

CHtmlCreator::CHtmlCreator()
{
}


CHtmlCreator::~CHtmlCreator()
{
}

QString CHtmlCreator::JoinWithHtmlNewLines(QStringList variables)
{
    return variables.join(HTMLNEWLINE);
}

QString CHtmlCreator::PrintVariableValue(const QString &variable, const QString &value)
{
    return QString("<i><b>%1</i></b> = %2").arg(variable, value);
}

QString CHtmlCreator::PrintHtmlSet(const QString& variableName, const QSet<QString>& elements, const QString& separator) {
    QString out = QString("<i><b>%1</b></i> = ").arg(variableName);
    out += qSetToQString(elements, separator);
    return out;
}

QString CHtmlCreator::ConvertTextToHtml(const QString& nonHtmlText)
{
    return nonHtmlText.toHtmlEscaped();;
}
