#ifndef CPDAWIDGET_H
#define CPDAWIDGET_H

#include <QObject>
#include <QWidget>
#include <widgets/fawidget.h>
#include <finite_machine/pushdownautomata.h>

class CPdaWidget : public FA_widget
{
public:
    CPdaWidget(QWidget *parent = 0);

public slots:
    void SetPda(CPushDownAutomata* pda);

protected:
  CPushDownAutomata* m_pa;
};

#endif // CPDAWIDGET_H
