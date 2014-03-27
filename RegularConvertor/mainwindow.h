#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include "finite_machine/diagramscene.h"
#include <QShortcut>
#include "finite_machine/fa_widget.h"
#include "reg_exp/regextextedit.h"
#include "reg_exp/regexpwidget.h"
#include "algorithms/regexptofa.h"
#include "algorithms/algorithmwidget.h"
#include "algorithms/algorithm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QShortcut* deleteShortCut;
    enum Conversions {none, RE_to_FA};
    AlgorithmWidget* alhgorithm_widget;

public slots:
    void testing_slot();
signals:
    void modeChanged(Algorithm::modes mode);
private slots:
    void myStatusbarShowMessage(QString message);
    void prepareREtoFA(RegExp* _re = new RegExp());
    void on_action_check_mode_triggered();
    void on_action_play_mode_triggered();
    void on_action_step_mode_triggered();

    void on_RE_FA_example1_triggered();

private:
    Ui::MainWindow *ui;
    DiagramScene *scene1;
    DiagramScene *scene2;
    QList<QGraphicsItem*> itemsToDelete;

    FA_widget* FA1_widget;
    FA_widget* FA2_widget;
    //RegExpTextEdit* regExpWidget;
    int statusBarTimeout;
    RegExpWidget* regExpWidget;
    //enum modes {CHECK_MODE, PLAY_MODE, STEP_MODE};
    Algorithm::modes mode;
    Conversions activeConversion;
    //void deleteSelected(QGraphicsScene *scene);

//    QToolButton *sceneBut;
};

#endif // MAINWINDOW_H
