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

#ifndef ARROW_H
#define ARROW_H

#define TEXT_DISTANCE 20
#define LINES_DISTANCE 10

#include <QGraphicsLineItem>
#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#else
    #include <QtGui>
#endif
#include "statenode.h"
#include "finiteautomata.h"
#include "symbolsinputdialog.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class StateNode;

class Arrow : public QGraphicsLineItem
{
public:
    ~Arrow();
    enum { Type = UserType + 2 };

    Arrow(StateNode *startItem, StateNode *endItem, FiniteAutomata* _FA, QStringList symbol,
    QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color) { myColor = color; }
    StateNode *startItem() const { return myStartItem; }
    StateNode *endItem() const { return myEndItem; }
    void addSymbol(QString symbol);
    //return true if there are no aditonal symbols
    bool removeSymbol(QString symbol);
    void updatePosition();
    StateNode *myStartItem;
    StateNode *myEndItem;
    QStringList symbols;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget = 0);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
     FiniteAutomata* FA;

     QString displayText;
     QColor myColor;
     QPolygonF arrowHead;
     QPolygonF selfArrowHead;
     long int debugCounter;
     QPointF textPos() const;
     QPolygonF SelfArrowHead();
     QPointF getDistancePoint() const;
     //Between same nodes are arrows with oposit direction
     bool arrowHasSibling() const;
     QPointF getStartItemPos() const;
     QPointF getEndItemPos()const;
     //pro vypocet jakym smerem se ma sipka posunout, je to proto, aby se opacne sipky mezi stejnymi
     //uzly neprekrivali
     QPointF perpendicularDifference(QLineF line, qreal distance)const;
     QRectF recalculateTextSpace() const;
 };

#endif


