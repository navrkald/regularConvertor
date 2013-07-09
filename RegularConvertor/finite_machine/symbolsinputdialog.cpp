#include "symbolsinputdialog.h"
#include "ui_symbolsinputdialog.h"
#include <QRegExpValidator>
#include <QPushButton>

SymbolsInputDialog::SymbolsInputDialog(QString initText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SymbolsInputDialog)
{
    ui->setupUi(this);
    okButton = this->ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    QRegExp listOfSymbols("^(\\s*\\S\\s*)?(\\s*,\\s*\\S\\s*\\s*)*,?\\s*$");
    this->alphabetValidator = new QRegExpValidator(listOfSymbols, this);
    ui->symbolsLineEdit->setText(initText);
    connect(this->ui->symbolsLineEdit,SIGNAL(textChanged(QString)),this,SLOT(symbolsEdited(QString)));
    connect(this,SIGNAL(accepted()),this,SLOT(DialogAccepted()));
}

SymbolsInputDialog::~SymbolsInputDialog()
{
    delete ui;
}

void SymbolsInputDialog::symbolsEdited(QString raw_text)
{
    int pos = 0;
    QValidator::State state =  this->alphabetValidator->validate(raw_text,pos);
    if(state == QValidator::Acceptable)
    {
        this->ui->symbolsLineEdit->setStyleSheet("QLineEdit{background: white;}");
        okButton->setEnabled(true);
    }
    else if (state == QValidator::Intermediate)
    {
        this->ui->symbolsLineEdit->setStyleSheet("QLineEdit{background: yellow;}");
        okButton->setEnabled(false);
    }
    else
    {
        this->ui->symbolsLineEdit->setStyleSheet("QLineEdit{background: red;}");
        okButton->setEnabled(false);
    }
    if(raw_text == "")
        okButton->setEnabled(false);
}

void SymbolsInputDialog::DialogAccepted()
{
    this->symbols = getSortedUniqueList(ui->symbolsLineEdit->text());
}

QStringList SymbolsInputDialog::getSortedUniqueList(QString raw_text)
{
    //this list can contain duplicite states
    //first replace sequence of white spaces by one space
    //second remove spaces
    //second split to strings

    //if ends with ',' then remove it
    if(raw_text.endsWith(','))
        raw_text.remove(raw_text.length()-1,1);
    QStringList raw_list_of_states = raw_text.simplified().replace(" ", "" ).split(",");
    QStringList unique_list_of_states = raw_list_of_states.toSet().toList();
    unique_list_of_states.sort();
    return unique_list_of_states;
}

void SymbolsInputDialog::on_addEpsilonButton_clicked()
{
    this->ui->symbolsLineEdit->insert("ε");
    this->ui->symbolsLineEdit->setFocus();
}
