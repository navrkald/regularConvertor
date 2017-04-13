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
          QGraphicsItem *parent, DiagramScene* _scene)
     : QGraphicsLineItem(parent)
 {
     m_symbols = symbols;
     m_displayText = this->m_symbols.join(", ");
     m_scene = _scene;
     m_FA = _FA;
     m_myStartItem = startItem;
     m_myEndItem = endItem;
     setFlag(QGraphicsItem::ItemIsSelectable, true);
     m_myColor = Qt::black;
     setPen(QPen(m_myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     m_debugCounter = 0;

     connect(this,SIGNAL(FA_changed(FiniteAutomata*)),m_scene,SIGNAL(FA_changed(FiniteAutomata*)));
 }

Arrow::Arrow(StateNode* startItem, StateNode* endItem, QGraphicsItem* parent, DiagramScene* _scene) :
    m_myStartItem(startItem), m_myEndItem(endItem),	QGraphicsLineItem(parent), m_scene(_scene)
{}

Arrow::~Arrow()
 {
    m_myStartItem->removeArrow(this);
    m_myEndItem->removeArrow(this);
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
	 path.addRect(GetDisplayTextRect());
     if(GetStartItemPos() == GetEndItemPos())
     {//jedna se o self smycku
         path.addEllipse(QPointF (GetStartItemPos().x(), GetStartItemPos().y() - NODE_RADIUS * 2),NODE_RADIUS *1,NODE_RADIUS *1);
     }
     else
     {//je to sipka mezi 2 ruznymi uzly
		 path.addPath(QGraphicsLineItem::shape());
         path.addPolygon(m_arrowHead);
     }
     return path;
 }

 void Arrow::addSymbol(QString symbol)
 {
     QSet <QString> symbolsSet = m_symbols.toSet();
     symbolsSet.insert(symbol);
     m_symbols = symbolsSet.toList();
     m_symbols.sort();
 }

 bool Arrow::removeSymbol(QString symbol)
 {
     m_symbols.removeOne(symbol);
     update();
     if(m_symbols.empty())
         return true;
     else
         return false;
 }



void Arrow::updatePosition()
{
    QLineF line(mapFromItem(m_myStartItem, 0, 0), mapFromItem(m_myEndItem, 0, 0));
    setLine(line);
}


void Arrow::EditArrow()
{
    SymbolsInputDialog inputDialog(m_symbols.join(", "));
    if(QDialog::Accepted == inputDialog.exec())
    {
        //Setup FA
        QStringList editedSymbols = inputDialog.symbols;
        QSet<QString> deletedSymbols = this->m_symbols.toSet() -  editedSymbols.toSet();
        QSet<QString> newSymbols = editedSymbols.toSet() - this->m_symbols.toSet();
        foreach(QString symbol,deletedSymbols)
        {
            m_FA->removeRule(ComputationalRules(startItem()->getName(),endItem()->getName(),symbol));
        }
        foreach(QString symbol,newSymbols)
        {
            m_FA->addRule(ComputationalRules(startItem()->getName(),endItem()->getName(),symbol));
        }
        //Setup this
        m_symbols  = editedSymbols;
        m_displayText = this->m_symbols.join(", ");
        emit FA_changed(m_FA);
    }
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen myPen = pen();
    myPen.setColor(m_myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);

	QRect textRectangle = GetDisplayTextRect();
    painter->drawText(textRectangle, Qt::AlignTop | Qt::AlignLeft,this->m_displayText);

    if (isSelected())
    {
        painter->setPen(QPen(m_myColor, 1, Qt::DashLine));
    }

    if(GetStartItemPos() == GetEndItemPos())
    {//jedna se o self smycku

        painter->drawEllipse(QPointF (GetStartItemPos().x(), GetStartItemPos().y() - NODE_RADIUS * 2),NODE_RADIUS *1,NODE_RADIUS * 1);
    }
    else
    {//je to sipka mezi 2 ruznymi uzly
        painter->setBrush(m_myColor);
        QLineF centerLine(GetStartItemPos(), GetEndItemPos());



        //centerLine.setLength ( centerLine.length ()  - myEndItem->getRadius());

        QPointF endPoint = centerLine.p2();
        QPointF startPoint = centerLine.p1();
        centerLine.setPoints(endPoint, startPoint);
        centerLine.setLength ( centerLine.length ()  - m_myEndItem->getRadius());


        setLine(centerLine);

        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);

        m_arrowHead.clear();
        m_arrowHead << line().p1() << arrowP1 << arrowP2;
        painter->drawLine(line());
        painter->drawPolygon(m_arrowHead);
        if (isSelected())
        {
            painter->setPen(QPen(m_myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }
    }
}

 void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
 {
		 EditArrow();
 }

 QRect Arrow::GetDisplayTextRect() const
 {
	 QSize textSize = GetDisplayTextSize();
	 QRect textRect;
     if(this->m_myStartItem == this->m_myEndItem)
     {
        textRect.setX(GetStartItemPos().x() - textSize.width()/ 2);
        textRect.setY(GetStartItemPos().y() - NODE_RADIUS * 2 - 1.5 * TEXT_DISTANCE - textSize.height());
     }
     else
     {
        QLineF centerLine(GetStartItemPos(), GetEndItemPos());

        QPointF centerPoint =  centerLine.pointAt(0.5);
        centerPoint -= PerpendicularDifference(QLineF(GetStartItemPos(),GetEndItemPos()),TEXT_DISTANCE);
        QPointF lineVector = GetEndItemPos() - GetStartItemPos();
        //          B
        //         ^
        // text   /
        //      A
        if(lineVector.x() < 0 && lineVector.y() > 0)
        {
         centerPoint.setX( centerPoint.x() - textSize.width());
         centerPoint.setY( centerPoint.y() - textSize.height());
        }
        //          B
        //         /
        // text   v
        //       A
        if(lineVector.x() > 0 && lineVector.y() > 0)
        {
         centerPoint.setX( centerPoint.x() - textSize.width());
         centerPoint.setY( centerPoint.y() + textSize.height());
        }
        textRect.setTopLeft(centerPoint.toPoint());
     }
     textRect.setSize(textSize);
     qDebug() << "Start item pos: " << startItem()->pos()
              << " End Item pos: " << endItem()->pos()
              << " Text rect :" << textRect
              << " Text rect size: " << textRect.size();
	 return textRect;
 }


QPointF Arrow::GetDistancePoint() const
{
    if(this->m_myStartItem == this->m_myEndItem)
    {
        return QPointF(GetStartItemPos().x(), GetStartItemPos().y() - NODE_RADIUS * 2 - 1.5 * TEXT_DISTANCE);
    }
    else
    {
        QLineF centerLine(GetStartItemPos(), GetEndItemPos());
        QPointF center_point =  centerLine.pointAt(0.5);
        QLineF tmpLine(center_point ,centerLine.p2());
        QLineF normalLine = tmpLine.normalVector();
        if(GetStartItemPos().y() > GetEndItemPos().y())
            normalLine.setLength(-TEXT_DISTANCE);
        else
            normalLine.setLength(TEXT_DISTANCE);
        return normalLine.p2();
    }
}

bool Arrow::ArrowHasSibling()const
{
    foreach(Arrow* arrow,this->m_myStartItem->arrows)
    {
        if(arrow->startItem() == this->endItem() && arrow->endItem() == this->startItem())
            return true;
    }
    return false;
}

QPointF Arrow::GetStartItemPos() const
{
    // If there is another arrow with oposite direction
    if(ArrowHasSibling())
        return startItem()->pos() - PerpendicularDifference(QLineF(startItem()->pos(),endItem()->pos()),LINES_DISTANCE);
    else
        return startItem()->pos();
}

QPointF Arrow::GetEndItemPos() const
{
    if(endItem() == startItem())
    {
        return startItem()->sceneBoundingRect().center();
    }
    else if(ArrowHasSibling())
    {
        return EllipseLineIntersection(endItem()->mapRectToScene(endItem()->elipseBoundingRect()), startItem()->sceneBoundingRect().center(), endItem()->mapRectToScene(endItem()->elipseBoundingRect()).center() - PerpendicularDifference(QLineF(startItem()->pos(),endItem()->pos()),LINES_DISTANCE));
    }
    else
    {
        return EllipseLineIntersection(endItem()->mapRectToScene(endItem()->elipseBoundingRect()), startItem()->sceneBoundingRect().center(), endItem()->mapRectToScene(endItem()->elipseBoundingRect()).center());
    }
}

// Perpendicular == kolmá
QPointF Arrow::PerpendicularDifference(QLineF line, qreal distance)const
{
    QLineF normalLine = line.normalVector();
    normalLine.setLength(distance);
    return normalLine.p2() - line.p1();
}

QSize Arrow::GetDisplayTextSize() const
{
    QFontMetrics metrics(qApp->font());
    return metrics.size(0, m_displayText);
}

//QPointF Arrow::intersectionPoint1(StateNode *circle, QLineF  *line) const

//{
//    float R= 20;
//    float x0 = (circle->mapToScene(circle->shape().controlPointRect().center())).x();
//    float y0 = (circle->mapToScene(circle->shape().controlPointRect().center())).y();
// if(line->x2() != line->x1())
//  {
//    QPointF yaxis_intersection;
//    line->intersect( QLineF(QPointF(0, 10000), QPointF(0, -10000)), &yaxis_intersection);
//    float a = (line->y2() - line->y1())/(line->x2() - line->x1());
//    float b = yaxis_intersection.y();
//    float A = 1 + a*a;
//    float B = 2*(a*b - a*y0 - x0);
//    float C = x0 * x0 + y0*y0 + b*b - 2*b*y0 - R*R;
//    float Q = B*B - 4*A*C;
//    if(Q > 0)
//     {
//       float s1 = (-1)*(B + sqrt(Q))/(2*A);
//       float s2 = (sqrt(Q) - B)/(2*A);
//       QPointF ps1(s1, a*s1 + b);
//       QPointF ps2(s2, a*s2 + b);
// if(contains(ps1))
//  return ps1;
// else
//  return ps2;
//     }
//     else
//     {
//       float s0 = (-1)*B/(2*A);
//       return QPointF(s0, a*s0 + b);
//     }
//   }
//  else
//   {
//     float x = line->x1();
//     // y*y - 2*y0*y + (x - x0)*(x - x0) + y0*y0 - R*R = 0
//     float C = (x - x0)*(x - x0) + y0*y0 - R*R;
//     float Q = 4*y0*y0 - 4*C;
//    if(Q > 0)
//     {
//       float s1 = y0 - sqrt(Q)/2;
//       float s2 = y0 + sqrt(Q)/2 ;
//       QPointF ps1(x, s1);
//       QPointF ps2(x, s2);
// if(contains(ps1))
//  return ps1;
// else
//  return ps2;
//     }
//     else
//     {
//       return QPointF(x, y0);
//     }
//   }
//}

QPointF Arrow::EllipseLineIntersection(QRectF elipse, QPointF p1, QPointF p2) const
{
/*

//elipse

M[m,n] --middle
a - width
b - height

   (x-m)^2        (y+n)^2
-------------- + --------- = 1
    a^2             b^2

//line

X(t) = X1+(X2-X1)t
Y(t) = Y2+(Y2-Y1)t
    

t^2(b^2(x2-x1)+a^2(y2-y1)) + t(-2b^2(x2-x1(x1-m)-2a^(y2-y1)(y1-n))) + ((x1-m)^2*b^2+(y1-n)^2*a^2+a^2b^2)
    ^                          ^                                              ^
    |                          |                                              |
t^2*A                        t*B                                              C
========================================================================================================
   B^2+-sqrt(B^2-4AC)
t=-------------------
         2A

*/

   qreal a = elipse.width()/2.0;
   qreal b = elipse.height()/2.0;
   qreal m = elipse.center().x();
   qreal n = elipse.center().y();
   qreal x1 = p1.x();
   qreal y1 = p1.y();
   qreal x2 = p2.x();
   qreal y2 = p2.y();

   qreal A = ( x2 - x1 ) * ( x2 - x1 ) * b*b + ( y2 - y1 ) * ( y2 - y1 ) * a*a;
   qreal B = ( 2*x1 * ( x2 - x1 ) -2 * ( x2 - x1 ) *m) * b*b + ( 2* y1 * ( y2 - y1 ) -2 * (y2-y1) * n ) * a*a;
   qreal C = b*b * ( x1*x1 + m*m -2*x1*m ) + a*a * (y1*y1 + n*n - 2*y1*n) - a*a*b*b;
   qreal D = B*B - 4*A*C;

   if(D < 0)
   {
       qFatal("Fatal error: EllipseLineIntersection() NEGATIVE Diskriminant!");
       exit(-1);
   }

   qreal t1 = (-B + qSqrt(D))/(2*A);
   QPoint intersect1 = QPoint(x1+(x2-x1)*t1,y1+(y2-y1)*t1);
   qreal t2 = (-B - qSqrt(D))/(2*A);
   QPoint intersect2 = QPoint(x1+(x2-x1)*t2,y1+(y2-y1)*t2);
   if((t1 >=0 && t1 <= 1 ) || (t2 >= 0 && t2 <= 1) )
   {
        if((t1 >=0 && t1 <= 1 ) && (t2 >= 0 && t2 <= 1))
            return (t1 < t2) ? intersect1 : intersect2;
        else if(t1 >=0 && t1 <= 1)
        {
            return intersect1;
        }
        else
        {
            return intersect2;
        }

   }
   else
   {
       // No intersect, returning end point p2;
       // Line is probably inside epilse
       return p2;
   }

}


