#include "editruledialog.h"
#include "ui_editruledialog.h"

editRuleDialog::editRuleDialog(QStringList _states, QStringList _alphabet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editRuleDialog)
{
    ui->setupUi(this);
    ui->fromComboBox->addItems(_states);
    ui->symbolComboBox->addItem(EPSILON);
    ui->symbolComboBox->addItems(_alphabet);
    ui->toComboBox->addItems(_states);
    this->from = ui->fromComboBox->itemText(0);
    this->symbol = ui->symbolComboBox->itemText(0);
    this->to = ui->toComboBox->itemText(0);
    //qDebug() <<"Returning from: "<<this->from;
    //qDebug() <<"Returning symbol: "<<this->symbol;
    //qDebug() <<"Returning to: "<<this->to;
}

editRuleDialog::~editRuleDialog()
{
    delete ui;
}

void editRuleDialog::setFrom(QString _from)
{
    int index = ui->fromComboBox->findText(_from,Qt::MatchExactly);
    ui->fromComboBox->setCurrentIndex(index);
    this->from = ui->fromComboBox->currentText();
}

void editRuleDialog::setSymbol(QString _symbol)
{
    int index = ui->symbolComboBox->findText(_symbol,Qt::MatchExactly);
    ui->symbolComboBox->setCurrentIndex(index);
    this->symbol = ui->symbolComboBox->currentText();
}

void editRuleDialog::setTo(QString _to)
{
    int index = ui->toComboBox->findText(_to,Qt::MatchExactly);
    ui->toComboBox->setCurrentIndex(index);
    this->to = ui->toComboBox->currentText();
}

QString editRuleDialog::getFrom()
{
    //QString from = ui->fromComboBox->itemData(1).toString();
    //qDebug() <<"Returning index at: " <<ui->fromComboBox->currentIndex();
    qDebug() <<"Returning from: "<<this->from;
    return this->from;
}

QString editRuleDialog::getSymbol()
{
    return this->symbol;//ui->symbolComboBox->itemData(ui->symbolComboBox->currentIndex()).toString();
}

QString editRuleDialog::getTo()
{
    //return ui->toComboBox->itemData(ui->toComboBox->currentIndex()).toString();
    return this->to;
}

void editRuleDialog::on_fromComboBox_activated(const QString &arg1)
{
    this->from = arg1;
}

void editRuleDialog::on_symbolComboBox_activated(const QString &arg1)
{
    this->symbol = arg1;
}

void editRuleDialog::on_toComboBox_activated(const QString &arg1)
{
    this->to = arg1;
}
