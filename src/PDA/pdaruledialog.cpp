#include "pdaruledialog.h"
#include "ui_pdaruledialog.h"

const QRegExp CPdaRuleDialog::m_pdaRuleRegExp("^\\s*(\\S+)\\s*,\\s*(\\S+)\\s*->\\s*(\\S+)\\s*((,\\s*(\\S+)\\s*)*)$");

CPdaRuleDialog::CPdaRuleDialog(QString startState, QString endState, QSet<CPDACompotutationalRule> pdaRules, QWidget *parent) :
	m_startState(startState), m_endState(endState), m_pdaRules(pdaRules), QDialog(parent), ui(new Ui::CPdaRuleDialog)
{
    ui->setupUi(this);

    // Fill rules edit box if there are any rules
    if(!m_pdaRules.isEmpty()){
        ui->m_ruleEdit->setText(CPDACompotutationalRule::ToArrowText(m_pdaRules));
    }
    ui->m_okButton->setEnabled(false);
    m_defaultUiText = ui->m_ruleEdit->toHtml();
    ui->m_ruleEdit->setTextBackgroundColor(Qt::yellow);
}

CPdaRuleDialog::~CPdaRuleDialog()
{
  delete ui;
}

void CPdaRuleDialog::on_m_ruleEdit_textChanged()
{
	QSet<CPDACompotutationalRule> tmpRules;
	bool isTextValid = true; // by default true because of delete all text to be empty m_pdaRules
	// Parse rules
    foreach(QString line, ui->m_ruleEdit->toPlainText().split('\n',QString::SkipEmptyParts))
    {
		// Skip whitespace lines
		line = line.trimmed();
		if (line.isEmpty())
			continue;

        QString popSymbol;
        QString inputSymbol;
        QVector<QString> pushSymbols;
        int pos = m_pdaRuleRegExp.indexIn(line);
        isTextValid = pos > -1;
        ui->m_okButton->setEnabled(isTextValid);
        ui->m_ruleEdit->setStyleSheet(isTextValid ? m_backgroundTransparentStr : m_backgroundYellowStr);
        ui->m_okButton->setEnabled(isTextValid);
        if (isTextValid)
        {
            popSymbol = m_pdaRuleRegExp.cap(1).trimmed();
            inputSymbol = m_pdaRuleRegExp.cap(2).trimmed();
			pushSymbols = TrimPushSymbols(m_pdaRuleRegExp.cap(4).split(",", QString::SkipEmptyParts));
            pushSymbols.push_front(m_pdaRuleRegExp.cap(3).trimmed());
			tmpRules.insert(CPDACompotutationalRule(m_startState, m_endState, inputSymbol, popSymbol, pushSymbols));
        }
        else
        {
            break;
        }
    }

    if(tmpRules.isEmpty()){
         ui->m_okButton->setEnabled(false);
	}
	
	if(isTextValid) {
		m_pdaRules = tmpRules;
	}

}

QVector<QString> CPdaRuleDialog::TrimPushSymbols(const QStringList& pushSymbols) {
	QVector<QString> pushSymbolsVector;
	foreach(const QString& pushSymbol, pushSymbols) {
		pushSymbolsVector.push_back(pushSymbol.trimmed());
	}
	return pushSymbolsVector;
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
