#include "pdawidget.h"
#include <PDA/pdadiagramscene.h>
#include <widgets/fawidget.h>
#include "ui_fawidget.h"

CPdaWidget::CPdaWidget(QWidget *parent) : FA_widget(parent)
{
	m_pda = new CPushDownAutomata();
	SetScene(new CPDADiagramScene(m_pda, ui->graphicsView));
    ui->tabWidget->removeTab(1);
}

CPushDownAutomata CPdaWidget::GetPda()
{
    return *m_pda;
}

void CPdaWidget::SetPda(CPushDownAutomata *pda)
{
    CPDADiagramScene* tmpPdaScene = dynamic_cast<CPDADiagramScene*>(m_scene);
    disconnect(tmpPdaScene, SIGNAL(PdaChangedSignal(CPushDownAutomata*)),
               this, SIGNAL(SignalPdaChanged(CPushDownAutomata*)));
	// clean(); TODO: clean formal model of PDA
	m_pda = pda;
    emit SignalSetPdaToScene(m_pda);
    connect(tmpPdaScene, SIGNAL(PdaChangedSignal(CPushDownAutomata*)),
            this, SIGNAL(SignalPdaChanged(CPushDownAutomata*)));
	emit SignalPdaChanged(m_pda);
}

void CPdaWidget::SetSceneSpecific(DiagramScene *scene)
{
	CPDADiagramScene* pdaDiagramScene = dynamic_cast<CPDADiagramScene*>(scene);

    // Connect rules
    connect(this,SIGNAL(AddEdges(QSet<CPDACompotutationalRule>)),
            pdaDiagramScene,SLOT(AddEdgesSlot(QSet<CPDACompotutationalRule>)));
    connect(this,SIGNAL(RemoveEdges(QSet<CPDACompotutationalRule>)),
            pdaDiagramScene,SLOT(RemoveEdgesSlot(QSet<CPDACompotutationalRule>)));

    //set FA also to scene
    connect(this,SIGNAL(SignalSetPdaToScene(CPushDownAutomata*)),
            pdaDiagramScene,SLOT(SetPdaSlot(CPushDownAutomata*)));

    // FA changed - scene notify
    connect(pdaDiagramScene,SIGNAL(PdaChangedSignal(CPushDownAutomata*)),
            this,SIGNAL(SignalPdaChanged(CPushDownAutomata*)));
}
