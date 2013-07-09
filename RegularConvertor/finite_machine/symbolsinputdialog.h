#ifndef SYMBOLSINPUTDIALOG_H
#define SYMBOLSINPUTDIALOG_H

#include <QDialog>
#include <QValidator>

namespace Ui {
class SymbolsInputDialog;
}

class SymbolsInputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SymbolsInputDialog(QString initText ,QWidget *parent = 0);
    QStringList symbols;
    ~SymbolsInputDialog();
private slots:
    void symbolsEdited(QString raw_text);
    void DialogAccepted();

    void on_addEpsilonButton_clicked();

private:
    Ui::SymbolsInputDialog *ui;
    QRegExpValidator *alphabetValidator;
    QPushButton *okButton;

    QStringList getSortedUniqueList(QString raw_text);
};

#endif // SYMBOLSINPUTDIALOG_H
