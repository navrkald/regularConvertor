#ifndef ALGORITHMWIDGET_H
#define ALGORITHMWIDGET_H

#include <QWidget>
#include "algorithmview.h"

namespace Ui {
class AlgorithmWidget;
}

class AlgorithmWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AlgorithmWidget(QWidget *parent = 0);
    ~AlgorithmWidget();
    AlgorithmView* getAlgorithmView();

private:
    Ui::AlgorithmWidget *ui;
};

#endif // ALGORITHMWIDGET_H
