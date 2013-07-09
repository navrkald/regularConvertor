/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QtGui>
 #include "arrow.h"
 #include <math.h>
#include <iostream>
 const qreal Pi = 3.14;

 Arrow::Arrow(StateNode *startItem, StateNode *endItem, FiniteAutomata* _FA, QStringList symbols,
          QGraphicsItem *parent, QGraphicsScene *scene)
     : QGraphicsLineItem(parent)
 {
     this->symbols = symbols;
     this->displayText = this->symbols.join(", ");
     FA = _FA;
     myStartItem = startItem;
     myEndItem = endItem;
     setFlag(QGraphicsItem::ItemIsSelectable, true);
     myColor = Qt::black;
     setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     debugCounter = 0;
 }

 Arrow::~Arrow()
 {
    myStartItem->removeArrow(this);
    myEndItem->removeArrow(this);
 }

 QRectF Arrow::boundingRect() const
 {
     qreal extra = (pen().width() + 20) / 2.0;

//     return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
//                                       line().p2().y() - line().p1().y()))
//         .normalized()
//         .adjusted(-extra, -extra, extra, extra);
    return shape().boundingRect().adjusted(-extra, -extra, extra, extra);
 }

 QPainterPath Arrow::shape() const
 {
     QPainterPath path;
     //this->displayText = symbols.join(", ");
     path.addText(textPos(),qApp->font(),displayText);

     if(getStartItemPos() == getEndItemPos())
     {//jedna se o self smycku
         path.addEllipse(QPointF (getStartItemPos().x(), getStartItemPos().y() - NODE_RADIUS * 2),NODE_RADIUS *1,NODE_RADIUS *1);
         //path.addPolygon();
         path.addText(textPos(),qApp->font(),displayText);
     }
     else
     {//je to sipka mezi 2 ruznymi uzly
         path = QGraphicsLineItem::shape();
         path.addText(textPos(),qApp->font(),displayText);
         path.addPolygon(arrowHead);
     }
     return path;
 }

 void Arrow::addSymbol(QString symbol)
 {
     QSet <QString> symbolsSet = symbols.toSet();
     symbolsSet.insert(symbol);
     symbols = symbolsSet.toList();
     symbols.sort();
 }

 bool Arrow::removeSymbol(QString symbol)
 {
     symbols.removeOne(symbol);
     update();
     if(symbols.empty())
         return true;
     else
         return false;
 }



void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
    std::cout<<"fce update position"<< std::endl;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
QWidget *)
{
    //if (myStartItem != myEndItem && myStartItem->collidesWithItem(myEndItem))
    //return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);

    this->displayText = symbols.join(", ");
    painter->drawText(textPos(),this->displayText);

    if(getStartItemPos() == getEndItemPos())
    {//jedna se o self smycku

        painter->drawEllipse(QPointF (getStartItemPos().x(), getStartItemPos().y() - NODE_RADIUS * 2),NODE_RADIUS *1,NODE_RADIUS * 1);
    }
    else
    {//je to sipka mezi 2 ruznymi uzly
        painter->setBrush(myColor);
        QLineF centerLine(getStartItemPos(), getEndItemPos());



        centerLine.setLength ( centerLine.length ()  - myEndItem->getRadius());

        QPointF endPoint = centerLine.p2();
        QPointF startPoint = centerLine.p1();
        centerLine.setPoints(endPoint, startPoint);
        centerLine.setLength ( centerLine.length ()  - myEndItem->getRadius());


        setLine(centerLine);

        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected())
        {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }
    }
}

 void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
 {
     SymbolsInputDialog inputDialog(symbols.join(", "));
    if(QDialog::Accepted == inputDialog.exec())
    {
        //Setup FA
        QStringList editedSymbols = inputDialog.symbols;
        QSet<QString> deletedSymbols = this->symbols.toSet() -  editedSymbols.toSet();
        QSet<QString> newSymbols = editedSymbols.toSet() - this->symbols.toSet();
        foreach(QString symbol,deletedSymbols)
        {
            FA->removeRule(ComputationalRules(startItem()->getName(),endItem()->getName(),symbol));
        }
        foreach(QString symbol,newSymbols)
        {
            FA->addRule(ComputationalRules(startItem()->getName(),endItem()->getName(),symbol));
        }

        //Setup this
        this->symbols  = editedSymbols;
        this->displayText = this->symbols.join(", ");
    }
 }

 QPointF Arrow::textPos() const
 {
     if(this->myStartItem == this->myEndItem)
     {
         return getDistancePoint();
     }
     else
     {
         QRectF text_rect = recalculateTextSpace();
         QLineF centerLine(getStartItemPos(), getEndItemPos());

         QPointF center_point =  centerLine.pointAt(0.5);
         center_point = center_point - perpendicularDifference(QLineF(getStartItemPos(),getEndItemPos()),TEXT_DISTANCE);
         QPointF lineVector = getEndItemPos() - getStartItemPos();
         //          B
         //         ^
         // text   /
         //      A
         if(lineVector.x() < 0 && lineVector.y() > 0)
         {
             center_point.setX( center_point.x() - text_rect.width());
             center_point.setY( center_point.y() - text_rect.height());
         }
         //          B
         //         /
         // text   v
         //       A
         if(lineVector.x() > 0 && lineVector.y() > 0)
         {
             center_point.setX( center_point.x() - text_rect.width());
             center_point.setY( center_point.y() + text_rect.height());
         }
         return center_point;
     }
 }


QPointF Arrow::getDistancePoint() const
{
    if(this->myStartItem == this->myEndItem)
    {
        return QPointF(getStartItemPos().x(), getStartItemPos().y() - NODE_RADIUS * 2 - 1.5 * TEXT_DISTANCE);
    }
    else
    {
        QLineF centerLine(getStartItemPos(), getEndItemPos());
        QPointF center_point =  centerLine.pointAt(0.5);
        QLineF tmpLine(center_point ,centerLine.p2());
        QLineF normalLine = tmpLine.normalVector();
        if(getStartItemPos().y() > getEndItemPos().y())
            normalLine.setLength(-TEXT_DISTANCE);
        else
            normalLine.setLength(TEXT_DISTANCE);
        return normalLine.p2();
    }
}

bool Arrow::arrowHasSibling()const
{
    foreach(Arrow* arrow,this->myStartItem->arrows)
    {
        if(arrow->startItem() == this->endItem() && arrow->endItem() == this->startItem())
            return true;
    }
    return false;
}

QPointF Arrow::getStartItemPos() const
{
    if(arrowHasSibling())
        return startItem()->pos() - perpendicularDifference(QLineF(startItem()->pos(),endItem()->pos()),LINES_DISTANCE);
    else
        return startItem()->pos();
}

QPointF Arrow::getEndItemPos() const
{
    if(arrowHasSibling())
       return endItem()->pos() - perpendicularDifference(QLineF(startItem()->pos(),endItem()->pos()),LINES_DISTANCE);
    else
        return endItem()->pos();
}


QPointF Arrow::perpendicularDifference(QLineF line, qreal distance)const
{
    QLineF normalLine = line.normalVector();
    normalLine.setLength(distance);
    return normalLine.p2() - line.p1();
}

QRectF Arrow::recalculateTextSpace() const
{
    //prevzato z c++ GUI programming wiht Qt 4, str. 203
    QFontMetrics metrics(qApp->font());
    return metrics.boundingRect(displayText);
}

