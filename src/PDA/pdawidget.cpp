#include "pdawidget.h"
#include <PDA/pdadiagramscene.h>
#include <widgets/fawidget.h>
#include "ui_fawidget.h"

CPdaWidget::CPdaWidget(QWidget *parent) : FA_widget(parent)
{
  m_pa = new CPushDownAutomata();
  SetScene(new CPDADiagramScene(m_pa, ui->graphicsView));
}
