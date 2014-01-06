#ifndef ALGORITHMWIDGET_H
#define ALGORITHMWIDGET_H

#include <QWidget>

namespace Ui {
class AlgorithmWidget;
}

class AlgorithmWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AlgorithmWidget(QWidget *parent = 0);
    ~AlgorithmWidget();
    
private:
    Ui::AlgorithmWidget *ui;
};

#endif // ALGORITHMWIDGET_H
