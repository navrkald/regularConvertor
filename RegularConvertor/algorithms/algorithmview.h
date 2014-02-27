#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include <qlistview.h>
#include <QWheelEvent>

class AlgorithmView : public QListView
{
public:
    AlgorithmView(QWidget *parent = 0);
protected:
    void wheelEvent(QWheelEvent *e);
};

#endif // ALGORITHMVIEW_H
