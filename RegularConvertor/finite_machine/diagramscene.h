#ifndef MYQGRAPHICSSCENE_H
#define MYQGRAPHICSSCENE_H

//pocet pokusu umistit uzel bez kolize s ostatnimi
#define NUM_OF_TRYES 1000

#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>

#include "finiteautomata.h"
#include "statenode.h"
class StateNode;
class Arrow;
class DiagramScene : public QGraphicsScene
{
   Q_OBJECT
signals:
    //void deleteSelected();
    void FA_changed(FiniteAutomata* FA);
    void sendErrorMessage(QString message);

public:
    DiagramScene(FiniteAutomata* _FA, QWidget *parent);
    //~DiagramScene();

    enum Mode { AddNode, MoveNode, DeleteNode, AddArrow};
    Mode actMode;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    Mode getMode();
    StateNode* startingState;
    Arrow* getArrow(StateNode* from, StateNode* to);
    //clean scene but not delete or change FA
    void emit_FA_changed(FiniteAutomata* FA);

public slots:
    void setFA(FiniteAutomata* FA);
    void setMode(Mode mode);
    void changeSelected();
    void deleteSelected();
    void addNodes(QSet<QString> nodes);
    void removeNodes(QSet<QString> nodes);
    void setStartNode(QString nodeName);
    void addEndingNodes(QSet <QString> nodes);
    void removeEndingNodes(QSet <QString> nodes);
    void addEdges(QSet <ComputationalRules> rules);
    void removeEdges(QSet <ComputationalRules> rules);
    void selectAll();

private:
    bool clicked;
    QGraphicsLineItem *actLine;
    FiniteAutomata* FA;
    QPoint randGeneratePos();
    //automaticaly place new node
    void addNode(QString node);
    StateNode* getNodeByName(QString nodeName);
    void clean();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // MYQGRAPHICSSCENE_H
