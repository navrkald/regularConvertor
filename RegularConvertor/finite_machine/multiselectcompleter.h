#ifndef MULTISELECTCOMPLETER_H
#define MULTISELECTCOMPLETER_H
#include <QCompleter>
#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

class MultiSelectCompleter : public QCompleter
{
    Q_OBJECT
public:
    MultiSelectCompleter( const QStringList& items, QObject* parent );
    MultiSelectCompleter(QObject *parent);
    ~MultiSelectCompleter();

public:
    void setItems(const QStringList& items);
    QString pathFromIndex( const QModelIndex& index ) const;
    QStringList splitPath( const QString& path ) const;

    QStringListModel* stringListModel;
};

#endif // MULTISELECTCOMPLETER_H
