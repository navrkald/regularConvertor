#ifndef CFGWIDGET_H
#define CFGWIDGET_H

#include <QWidget>
#include <CFG/contextfreegrammar.h>

namespace Ui {
class CCfgWidget;
}

class CCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CCfgWidget(QWidget *parent = 0);
    void SetCaption(const QString& caption);
    ~CCfgWidget();
    void SetCfg(const CContextFreeGrammar& cfg);


private slots:
  void on_m_BackusNaurFormTextEdit_textChanged();

private:
    Ui::CCfgWidget *ui;
    CContextFreeGrammar m_cfg;
    void SetBackgroundColor(const Qt::GlobalColor& color);
};

#endif // CFGWIDGET_H
