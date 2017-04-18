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
    void FA_changed(FiniteAutomata* m_fa);
    void sendStatusBarMessage(QString message);

public:
    DiagramScene(FiniteAutomata* _FA, QWidget *parent);
    //~DiagramScene();

    enum Mode { AddNodeMode, MoveNodeMode, DeleteNodeMode, AddArrowMode};
    Mode actMode;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    Mode getMode();
    StateNode* startingState;
    Arrow* getArrow(StateNode* from, StateNode* to);
    //clean scene but not delete or change FA
    void emit_FA_changed(FiniteAutomata* m_fa);

    void SetStartState(QString nodeName);
    void SetEndingState(QString nodeName, bool isEnding);
    void SetNodeCoordinates(QString nodeName, QPoint position);
    bool CanSetNodeName(QString newName);
    void RenameNode(QString oldName, QString newName);
public slots:
    void SetFa(FiniteAutomata* m_fa);
    void SetMode(Mode mode);
    void ChangeSelected();
    void DeleteSelected();
    void AddNodes(QSet<QString> nodes, QMap<QString,QPoint> coordinates = QMap<QString,QPoint>());
    void RemoveNodes(QSet<QString> nodes);
    void SetStartNode(QString nodeName);
    void AddEndingNodes(QSet <QString> nodes);
    void RemoveEndingNodes(QSet <QString> nodes);
    void AddEdges(QSet <ComputationalRules> rules);
    void RemoveEdges(QSet <ComputationalRules> rules);
    void SelectAll();

private:
    FiniteAutomata* m_fa;

protected:
    bool clicked;
    QGraphicsLineItem *actLine;
    QPoint randGeneratePos();
    //automaticaly place new node
    void addNode(QString node, QPoint point);
    StateNode* getNodeByName(QString nodeName);
    void clean();
    virtual void AddArrow(StateNode *startItem, StateNode *endItem);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    StateNode *CreateStateNode(QString nodeName = "");
};

#endif // MYQGRAPHICSSCENE_H
