#ifndef ALGORITHMWIDGET_H
#define ALGORITHMWIDGET_H

#include <QWidget>
#include "algorithmview.h"
#include "algorithm.h"
#include <QSpacerItem>

namespace Ui {
class AlgorithmWidget;
}

class AlgorithmWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AlgorithmWidget(Algorithm::modes _mode, QWidget *parent = 0);
    ~AlgorithmWidget();
    AlgorithmView* getAlgorithmView();
    Algorithm::modes mode;

    void hideWidgets(QList<QWidget*> widgets);
    void showWidgets(QList<QWidget*> widgets);

    void hideSpacer(QSpacerItem * s);
    void showSpacer(QSpacerItem * s);

    bool showSolution;

signals:
    void nextPressed();
    void prewPressed();
    void playPressed(int mil_sec);
    void stopPressed();
    void checkSolutionPressed();
    void showCorrectSolutionPressed();
    void showUserSolutionPressed();


public slots:
    void emitPlay();
    void setWidgets(Algorithm::modes mode);


private slots:
    void on_showButton_clicked();

private:
    Ui::AlgorithmWidget *ui;
};

#endif // ALGORITHMWIDGET_H
