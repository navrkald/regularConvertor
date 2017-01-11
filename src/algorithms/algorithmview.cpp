#include "algorithmview.h"

AlgorithmView::AlgorithmView(QWidget *)
{
    //setResizeMode(QListView::Adjust	);
}

void AlgorithmView::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers().testFlag(Qt::ControlModifier))
    {
        this->resize(this->size()*2);
    }
    else
    {
        QListView::wheelEvent(e);
    }

}
