#ifndef EDITRULEDIALOG_H
#define EDITRULEDIALOG_H

#include <QDialog>
#include <QtCore>
#define EPSILON "ε"

namespace Ui {
class editRuleDialog;
}

class editRuleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit editRuleDialog(QStringList _states, QStringList _alphabet, QWidget *parent = 0);
    ~editRuleDialog();
    void setFrom(QString _from);
    void setSymbol(QString _symbol);
    void setTo(QString _to);
    QString getFrom();
    QString getSymbol();
    QString getTo();
private slots:
    void on_fromComboBox_activated(const QString &arg1);

    void on_symbolComboBox_activated(const QString &arg1);

    void on_toComboBox_activated(const QString &arg1);

private:
    Ui::editRuleDialog *ui;
    QString from;
    QString symbol;
    QString to;
};

#endif // EDITRULEDIALOG_H
