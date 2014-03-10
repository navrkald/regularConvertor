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
signals:
    void nextPressed();
    void prewPressed();
    void playPressed(int mil_sec);
    void stopPressed();

public slots:
    void emitPlay();

private:
    Ui::AlgorithmWidget *ui;
};

#endif // ALGORITHMWIDGET_H
