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

void CPdaWidget::SetPda(CPushDownAutomata *pda)
{
    CPDADiagramScene* tmpScene = dynamic_cast<CPDADiagramScene*>(m_scene);
    disconnect(tmpScene, SIGNAL(PdaChangedSignal(CPushDownAutomata*)), this, SIGNAL(SignalPdaChanged(CPushDownAutomata*)));
	// clean(); TODO: clean formal model of PDA
	m_pda = pda;
	emit SignalPdaChanged(m_pda);
    connect(tmpScene, SIGNAL(PdaChangedSignal(CPushDownAutomata*)), this, SIGNAL(SignalPdaChanged(CPushDownAutomata*)));
	emit SignalPdaChanged(m_pda);

    // TODO: setup PDA
}

void CPdaWidget::SetSceneSpecific(DiagramScene *scene)
{
	/*
	CPDADiagramScene* pdaDiagramScene = dynamic_cast<CPDADiagramScene*>(scene);

    // Connect rules
    connect(this,SIGNAL(addEdges(QSet<ComputationalRules>)),this->m_scene,SLOT(addEdges(QSet<ComputationalRules>)));
    connect(this,SIGNAL(removeEdges(QSet<ComputationalRules>)),this->m_scene,SLOT(removeEdges(QSet<ComputationalRules>)));

    //set FA also to scene
    connect(this,SIGNAL(setFA_signalToScene(FiniteAutomata*)),this->m_scene,SLOT(setFA(FiniteAutomata*)));

    // FA changed - scene notify
    connect(m_scene,SIGNAL(FA_changed(FiniteAutomata*)),this,SIGNAL(FA_changed(FiniteAutomata*)));
	*/	
}
