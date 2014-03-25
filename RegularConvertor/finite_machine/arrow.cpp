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
          QGraphicsItem *parent, QGraphicsScene* _scene)
     : QGraphicsLineItem(parent)
 {
     this->symbols = symbols;
     this->displayText = this->symbols.join(", ");
     scene = _scene;
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
    //std::cout<<"fce update position"<< std::endl;
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



        //centerLine.setLength ( centerLine.length ()  - myEndItem->getRadius());

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
            FA->removeSymbol(symbol);
        }
        foreach(QString symbol,newSymbols)
        {
            FA->addRule(ComputationalRules(startItem()->getName(),endItem()->getName(),symbol));
            FA->addSymbol(symbol);
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
//         qDebug() << getStartItemPos() << getEndItemPos();
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
    if(arrowHasSibling()) {

        return endItem()->pos() - perpendicularDifference(QLineF(startItem()->pos(),endItem()->pos()),LINES_DISTANCE);
    }
    else
    {
        //QRectF rect = QRectF(endItem()->elipseBoundingRect().topLeft(), endItem()->pos());
        return EllipseLineIntersection(endItem()->sceneBoundingRect(), startItem()->sceneBoundingRect().center(), endItem()->sceneBoundingRect().center());
    }
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

QPointF Arrow::intersectionPoint1(StateNode *circle, QLineF  *line) const

{



    float R= 20;



    float x0 = (circle->mapToScene(circle->shape().controlPointRect().center())).x();

    float y0 = (circle->mapToScene(circle->shape().controlPointRect().center())).y();





 if(line->x2() != line->x1())

  {

    QPointF yaxis_intersection;

    line->intersect( QLineF(QPointF(0, 10000), QPointF(0, -10000)), &yaxis_intersection);



    float a = (line->y2() - line->y1())/(line->x2() - line->x1());

    float b = yaxis_intersection.y();



    float A = 1 + a*a;

    float B = 2*(a*b - a*y0 - x0);

    float C = x0 * x0 + y0*y0 + b*b - 2*b*y0 - R*R;



    float Q = B*B - 4*A*C;

    if(Q > 0)

     {

       float s1 = (-1)*(B + sqrt(Q))/(2*A);

       float s2 = (sqrt(Q) - B)/(2*A);

       QPointF ps1(s1, a*s1 + b);

       QPointF ps2(s2, a*s2 + b);



 if(contains(ps1))

  return ps1;

 else

  return ps2;

     }

     else

     {

       float s0 = (-1)*B/(2*A);

       return QPointF(s0, a*s0 + b);

     }

   }

  else

   {

     float x = line->x1();

     // y*y - 2*y0*y + (x - x0)*(x - x0) + y0*y0 - R*R = 0



     float C = (x - x0)*(x - x0) + y0*y0 - R*R;

     float Q = 4*y0*y0 - 4*C;



    if(Q > 0)

     {

       float s1 = y0 - sqrt(Q)/2;

       float s2 = y0 + sqrt(Q)/2 ;

       QPointF ps1(x, s1);

       QPointF ps2(x, s2);



 if(contains(ps1))

  return ps1;

 else

  return ps2;

     }

     else

     {

       return QPointF(x, y0);

     }

   }



}

QPointF Arrow::findEllipseSegmentIntersections(QRectF rect, QPointF pt1, QPointF pt2, bool segment_only) const
{
    // If the ellipse or line segment are empty, return no intersections.
    if (rect.width() == 0 || rect.height() == 0 || ((pt1.x() == pt2.x()) && (pt1.y() == pt2.y())))
        return QPointF(0,0);

    // Make sure the rectangle has non-negative width and height.
    if (rect.width() < 0)
    {
        rect.setX(rect.left());
        rect.setWidth(-rect.width());
    }
    if (rect.height() < 0)
    {
        rect.setY(rect.bottom());
        rect.setHeight(-rect.height());
    }

    // Translate so the ellipse is centered at the origin.
    qreal cx = rect.left() + rect.width() / 2.0;
    qreal cy = rect.top() - rect.height() / 2.0;
    rect.setX(rect.x() - cx);
    rect.setY(rect.y() - cy);
    pt1.setX(pt1.x() - cx);
    pt1.setY(pt1.y() - cy);
    pt2.setX(pt2.x() - cx);
    pt2.setY(pt2.y() - cy);

    // Get the semimajor and semiminor axes.
    qreal a = rect.width() / 2;
    qreal b = rect.height() / 2;

    // Calculate the quadratic parameters.
    qreal A = (pt2.x() - pt1.x()) * (pt2.x() - pt1.x()) / a / a +
            (pt2.y() - pt1.y()) * (pt2.y() - pt1.y()) / b / b;
    qreal B = 2 * pt1.x() * (pt2.x() - pt1.x()) / a / a +
            2 * pt1.y() * (pt2.y() - pt1.y()) / b / b;
    qreal C = pt1.x() * pt1.x() / a / a + pt1.y() * pt1.y() / b / b - 1;

    // Make a list of t values.
    QList<qreal> t_values;

    // Calculate the discriminant.
    float discriminant = B * B - 4 * A * C;
    if (discriminant == 0)
    {
        // One real solution.
        t_values.append(-B / 2 / A);
    }
    else if (discriminant > 0)
    {
        // Two real solutions.
        t_values.append((qreal)((-B + qSqrt(discriminant)) / 2 / A));
        t_values.append((qreal)((-B - qSqrt(discriminant)) / 2 / A));
    }
    else
    {
        qDebug() << "Fatal error: Discriminant is negative!";
        exit(-1);
    }

    // Convert the t values into points.
    QList<QPointF> points;
    foreach (qreal t, t_values)
    {
        // If the points are on the segment (or we
        // don't care if they are), add them to the list.
        if (!segment_only || ((t >= 0.0) && (t <= 1.0)))
        {
            qreal x = pt1.x() + (pt2.x() - pt1.x()) * t + cx;
            qreal y = pt1.y() + (pt2.y() - pt1.y()) * t + cy;
            points.append(QPointF(x, y));
        }
    }

    // Return the points.
   // qDebug() << "pp: " << points << endl;
    return points.first();
}
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
       qDebug() << "Fatal eroor: EllipseLineIntersection() NEGATIVE Diskriminant!";
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
       qDebug() << "Fatal eroor: No intersect, returning point [0,0]";
       return QPoint();
   }

}


