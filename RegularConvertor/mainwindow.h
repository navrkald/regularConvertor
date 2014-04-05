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
    enum Conversions {none, RE_to_FA, REMOVE_EPSILON};
    AlgorithmWidget* alhgorithm_widget;
    RegExpToFA* reg_exp_algorithm;

signals:
    void modeChanged(Algorithm::modes mode);
private slots:
    void myStatusbarShowMessage(QString message);
    void prepareREtoFA(RegExp* _re = new RegExp());
    void prepareRemoveEpsilon();

    void on_action_check_mode_triggered();
    void on_action_play_mode_triggered();
    void on_action_step_mode_triggered();

    void setRE_FA_example(RegExp* _re);

    void on_RE_FA_example1_triggered();

    void on_RE_FA_example0_triggered();

    void on_RE_FA_example2_triggered();

    void on_RE_FA_example3_triggered();

    void on_RE_FA_example4_triggered();

    void on_RE_FA_example5_triggered();

    void on_RE_FA_example6_triggered();

    void on_RE_FA_example7_triggered();

    void on_RE_FA_example8_triggered();

    void on_RE_FA_example9_triggered();

private:
    Ui::MainWindow *ui;
    QWidget* regExpToFA_widget;
    QWidget* removeEpsilon_widget;

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

    //regExp to FA basic components
    FA_widget* fa_widget_left;
    FA_widget* fa_widget_center;
    FA_widget* fa_widget_right;
    RegExpWidget* reg_exp_widget;


    void prepareREtoFA_GUI();
    void prepareRemoveEpsilon_GUI();

};

#endif // MAINWINDOW_H
