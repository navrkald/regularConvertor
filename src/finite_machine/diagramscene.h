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
    void FA_changed(FiniteAutomata* fa);
    void sendStatusBarMessage(QString message);

public:
    DiagramScene(FiniteAutomata* _FA, QWidget *parent);

    enum Mode { AddNodeMode, MoveNodeMode, DeleteNodeMode, AddArrowMode};
    Mode actMode;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    Mode getMode();
    StateNode* startingState;
    Arrow* getArrow(StateNode* from, StateNode* to);

    virtual void AddNewState(QString nodeName, QPoint position = QPoint());
    virtual void SetStartState(QString nodeName, bool emitFaChanged = true);
    virtual void SetEndingState(QString nodeName, bool isEnding);
    virtual void SetNodeCoordinates(QString nodeName, QPoint position);
    virtual bool CanSetNodeName(QString newName);
    virtual void RenameNode(QString oldName, QString newName);
public slots:
    void SetFa(FiniteAutomata* fa);
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
    virtual void RemoveStateFromFiniteAutomata(QString state);
    virtual void AddArrow(StateNode *startItem, StateNode *endItem);
    virtual void CheckArrowTypeAndDelete(QGraphicsItem* graphicsItem, QList<QGraphicsItem*>& items);
    virtual void EmitFiniteAutomataChanged();
    virtual QString CreateNodeUniqueName();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    StateNode *CreateStateNode(QString nodeName = "");

};

#endif // MYQGRAPHICSSCENE_H
