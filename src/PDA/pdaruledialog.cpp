#include "pdaruledialog.h"
#include "ui_pdaruledialog.h"

const QRegExp CPdaRuleDialog::m_pdaRuleRegExp("^\\s*(\\S+)\\s*,\\s*(\\S+)\\s*->\\s*(\\S+)\\s*((,\\s*(\\S+)\\s*)*)$");

CPdaRuleDialog::CPdaRuleDialog(QString startState, QString endState, QSet<CPDACompotutationalRule> pdaRules, QWidget *parent) :
	m_startState(startState), m_endState(endState), m_pdaRules(pdaRules), QDialog(parent), ui(new Ui::CPdaRuleDialog)
{
    m_defaultUiText = ui->m_ruleEdit->toHtml();
	// Fill rules edit box if there are any rules
	if(!m_pdaRules.isEmpty()){
		ui->m_ruleEdit->setText(CPDACompotutationalRule::ToArrowText(m_pdaRules));
  }
  ui->setupUi(this);
}

CPdaRuleDialog::~CPdaRuleDialog()
{
  delete ui;
}

void CPdaRuleDialog::on_m_ruleEdit_textChanged()
{


    // Parse rules
    foreach(QString line, ui->m_ruleEdit->toPlainText().split('\n',QString::SkipEmptyParts))
    {
        QString popSymbol;
        QString inputSymbol;
        QVector<QString> pushSymbols;
        int pos = m_pdaRuleRegExp.indexIn(line);//, ddd, eee");
        bool isTextValid = pos > -1;
        ui->m_okButton->setEnabled(isTextValid);
        SetBackgroundColor(isTextValid ? Qt::transparent : Qt::yellow);
        ui->m_okButton->setEnabled(isTextValid);
        if (isTextValid)
        {
            popSymbol = m_pdaRuleRegExp.cap(1);
            inputSymbol = m_pdaRuleRegExp.cap(2);
            pushSymbols = m_pdaRuleRegExp.cap(4).split(",",QString::SkipEmptyParts).toVector();
            pushSymbols.push_front(m_pdaRuleRegExp.cap(3));
            m_pdaRules.insert(CPDACompotutationalRule(m_startState, m_endState, inputSymbol, popSymbol, pushSymbols));
        }
        else
        {
            break;
        }
    }
}

void CPdaRuleDialog::SetBackgroundColor(const Qt::GlobalColor &color)
{
  QPalette palette = ui->m_ruleEdit->palette();
  palette.setColor(QPalette::Base, color);
  ui->m_ruleEdit->setPalette(palette);
}

void CPdaRuleDialog::on_m_cancelBut_clicked()
{
	reject();
}

void CPdaRuleDialog::on_m_okButton_clicked()
{
	accept();
}

void CPdaRuleDialog::on_m_addEpsilonBut_clicked()
{
	ui->m_ruleEdit->textCursor().insertText(EPSILON);
	this->ui->m_ruleEdit->setFocus();
}
