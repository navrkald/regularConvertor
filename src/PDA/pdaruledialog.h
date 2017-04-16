#ifndef PDARULEDIALOG_H
#define PDARULEDIALOG_H

#include <QDialog>
#include <QValidator>
#include <finite_machine/pdacompotutationalrules.h>

namespace Ui {
  class CPdaRuleDialog;
}

class CPdaRuleDialog : public QDialog
{
  Q_OBJECT

public:
	explicit CPdaRuleDialog(QString startState, QString endState, QSet<CPDACompotutationalRule> pdaRules, QWidget *parent = 0);
  ~CPdaRuleDialog();
  QSet<CPDACompotutationalRule> GetPdaRules() { return m_pdaRules; }
private slots:
  void on_m_ruleEdit_textChanged();

	void on_m_cancelBut_clicked();

	void on_m_okButton_clicked();

	void on_m_addEpsilonBut_clicked();

protected:
  static const QRegExp m_pdaRuleRegExp;
protected:
    QString m_defaultUiText;
    QString m_startState;
    QString m_endState;
    QSet<CPDACompotutationalRule> m_pdaRules;
    const QString m_backgroundYellowStr = "background-color:yellow ";
    const QString m_backgroundTransparentStr = "background-color:transparent ";

private:
  Ui::CPdaRuleDialog *ui;
};

#endif // PDARULEDIALOG_H
