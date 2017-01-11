#include "cfgwidget.h"
#include "ui_cfgwidget.h"
#include <error.h>

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

void CCfgWidget::on_m_BackusNaurFormTextEdit_textChanged()
{
  if(E_NO_ERROR == m_cfg.GetFromString(ui->m_BackusNaurFormTextEdit->toPlainText())){
    ui->m_nonterminalLabel->setText("N = { " + m_cfg.NonTerminalAlphabetToString() + "} is finite alphabet of non-terminals");
    ui->m_terminalLabel->setText("T = { " + m_cfg.TerminalAlphabetToString() + "} is finite alphabet of terminals");
    ui->m_firstNonTerminal->setText("S" + m_cfg.GetStartNonTerminal() + " is first nonterminal");
    SetBackgroundColor(Qt::transparent);
  }
  else{
    SetBackgroundColor(Qt::yellow);
    }
}

void CCfgWidget::SetBackgroundColor(const Qt::GlobalColor &color)
{
  QPalette palette = ui->m_BackusNaurFormTextEdit->palette();
  palette.setColor(QPalette::Base, color);
  ui->m_BackusNaurFormTextEdit->setPalette(palette);
}
