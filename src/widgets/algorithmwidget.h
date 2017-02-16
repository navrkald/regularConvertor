#ifndef ALGORITHMWIDGET_H
#define ALGORITHMWIDGET_H

#include <QWidget>
#include "algorithms/algorithmview.h"
#include "algorithms/algorithm.h"
#include <QSpacerItem>

namespace Ui {
class AlgorithmWidget;
}

class CAlgorithmWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CAlgorithmWidget(QWidget *parent = 0);
    explicit CAlgorithmWidget(CAlgorithm::modes _mode, QWidget *parent = 0);
    ~CAlgorithmWidget();
    AlgorithmView* getAlgorithmView();
    CAlgorithm::modes m_mode;

    void SetCaption(const QString& caption);
    void hideWidgets(QList<QWidget*> widgets);
    void showWidgets(QList<QWidget*> widgets);

    void hideSpacer(QSpacerItem * s);
    void showSpacer(QSpacerItem * s);
    void enableShowButton();
    void disableShowButton();

    bool showSolution;

signals:
    void nextPressed();
    void prewPressed();
    void playPressed(int mil_sec);
    void stopPressed();
    void checkSolutionPressed();
    void showCorrectSolutionPressed();
    void showUserSolutionPressed();
    void beginPressed();
    void endPressed();

public slots:
    void emitPlay();
    void SetMode(CAlgorithm::modes mode);
    void disableNext();
    void enableNext();
    void disablePrev();
    void enablePrev();



private slots:
    void on_showButton_clicked();

private:
    Ui::AlgorithmWidget *ui;
};

#endif // ALGORITHMWIDGET_H
