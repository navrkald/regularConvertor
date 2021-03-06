#include "cfgwidget.h"
#include "ui_cfgwidget.h"
#include <error.h>
#include <algorithms/constants.h>

CCfgWidget::CCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCfgWidget)
{
    ui->setupUi(this);
}

void CCfgWidget::SetCaption(const QString &caption)
{
    ui->m_cfgWidgetCaption->setText(caption);
}

CCfgWidget::~CCfgWidget()
{
    delete ui;
}

void CCfgWidget::SetCfg(const CContextFreeGrammar &cfg)
{
    m_cfg = cfg;
    ui->m_BackusNaurFormTextEdit->setPlainText(m_cfg.ToBackusNaurForm());
}

CContextFreeGrammar CCfgWidget::GetCfg()
{
    return m_cfg;
}

void CCfgWidget::on_m_addEpsilonButton_clicked()
{
	ui->m_BackusNaurFormTextEdit->textCursor().insertText(EPSILON);
	this->ui->m_BackusNaurFormTextEdit->setFocus();
}

void CCfgWidget::on_m_BackusNaurFormTextEdit_textChanged()
{
	if(E_NO_ERROR == m_cfg.GetFromBackusNaurForm(ui->m_BackusNaurFormTextEdit->toPlainText())){
		ui->m_nonterminalLabel->setText("N = { " + m_cfg.NonTerminalAlphabetToString() + "} is finite alphabet of non-terminals");
		ui->m_terminalLabel->setText("T = { " + m_cfg.TerminalAlphabetToString() + "} is finite alphabet of terminals");
		ui->m_firstNonTerminal->setText("S = " + m_cfg.GetStartNonTerminal() + " is first nonterminal");
		emit CfgChanged(m_cfg);
		ui->m_BackusNaurFormTextEdit->setStyleSheet(m_backgroundTransparentStr);
	}
	else{
		ui->m_BackusNaurFormTextEdit->setStyleSheet(m_backgroundYellowStr);
	}
}
