#include "diagramscene.h"
#include "statenode.h"
#include <iostream>

DiagramScene::DiagramScene(QWidget *parent) : QGraphicsScene(parent)
{
    this->actMode = AddNode;
    clicked = false;
    actLine = 0;
    //connect(this, SIGNAL(selectionChanged()), this, SLOT(changeSelected()));
}


void DiagramScene::setMode(DiagramScene::Mode mode)
{
    this->actMode = mode;

}


void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (mouseEvent->button() != Qt::LeftButton)
         return;
     clicked = true;
     StateNode* newNode;
     switch(this->actMode)
     {
        case AddNode:
             newNode = new StateNode(this);
             this->addItem(newNode);
             newNode->setPos(mouseEvent->scenePos());
             break;
        case AddArrow:
             actLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                         mouseEvent->scenePos()));
             actLine->setPen(QPen(Qt::black, 2));
             addItem(actLine);
             break;
        case MoveNode:
             break;
        case DeleteNode:
            break;
     }

//         QBrush redBrush(Qt::red);
//         QPen   bluePen(Qt::blue);
//         QPointF pos = mouseEvent->scenePos();
//         QGraphicsEllipseItem* elipse = this->addEllipse(pos.x()-25.0,pos.y()-25.0,50,50,bluePen,redBrush);
//         elipse->setFlag(QGraphicsItem::ItemIsMovable, true);
//         elipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
//         elipse->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
     QGraphicsScene::mousePressEvent(mouseEvent);
 }

 void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (actMode == AddArrow && actLine != 0)
     {
         QLineF newLine(actLine->line().p1(), mouseEvent->scenePos());
         actLine->setLine(newLine);
     }
     else //if (actMode == MoveNode)
         QGraphicsScene::mouseMoveEvent(mouseEvent);
}

 void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
    clicked = false;

     if (actLine != 0 && actMode == AddArrow)
     {
         QList<QGraphicsItem *> startItems = items(actLine->line().p1());
         if (startItems.count() && startItems.first() == actLine)
             startItems.removeFirst();

         QList<QGraphicsItem *> endItems = items(actLine->line().p2());
         if (endItems.count() && endItems.first() == actLine)
             endItems.removeFirst();

         removeItem(actLine);
         delete actLine;

         if (startItems.count() > 0 && endItems.count() > 0
            && 0 != qgraphicsitem_cast<StateNode *>(startItems.first())
            && 0 != qgraphicsitem_cast<StateNode *>(endItems.first())
            && startItems.first() != endItems.first())
         {
             std::cout <<"vypise nevypise"<< std::endl;
             StateNode *startItem = qgraphicsitem_cast<StateNode *>(startItems.first());
             StateNode *endItem = qgraphicsitem_cast<StateNode *>(endItems.first());
             Arrow *arrow = new Arrow(startItem, endItem);
             //arrow->setColor(myLineColor);
             startItem->addArrow(arrow);
             endItem->addArrow(arrow);
             arrow->setZValue(-1000.0);   //posun na pozadi
             addItem(arrow);

             arrow->updatePosition();
         }
     }
     actLine = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
 }

 //pokud jsme v modu ruseni zulu pak po oznaceni uzlu ho zrus
// void DiagramScene::changeSelected()
// {
//     if(actMode == DeleteNode && !this->selectedItems().isEmpty())
//     {
//         std::cout<<"call DeleteSelected()"<<std::endl;
//         deleteSelected();
//     }
// }

 //void DiagramScene::deleteSelected()
 //{
    // foreach (QGraphicsItem *item, this->selectedItems())
    // {
          //if (item->type() == StateNode::Type) {
              //qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
          //}
       //   removeItem(item);

          //TODO proc pada na tomto prikazu???????
          //delete item;
     //}
 //}

 DiagramScene::Mode DiagramScene::getMode()
 {
     return actMode;
 }


