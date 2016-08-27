#include "pdawidget.h"
#include <PDA/pdadiagramscene.h>
#include <ui_fa_widget.h>

CPdaWidget::CPdaWidget(QWidget *parent) : FA_widget(parent)
{
  m_pa = new CPushDownAutomata();
  SetScene(new CPDADiagramScene(m_pa, ui->graphicsView));
}
