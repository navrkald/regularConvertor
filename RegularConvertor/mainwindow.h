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
#include "algorithms/removeepsilon.h"

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



signals:
    void modeChanged(Algorithm::modes mode);
private slots:
    void myStatusbarShowMessage(QString message);

    void on_action_check_mode_triggered();
    void on_action_play_mode_triggered();
    void on_action_step_mode_triggered();

    //
    // RegExp to FA
    //
    void prepareREtoFA(RegExp* _re = new RegExp());
    void setRE_FA_example(RegExp* _re);
    void on_RE_FA_example0_triggered();
    void on_RE_FA_example1_triggered();
    void on_RE_FA_example2_triggered();
    void on_RE_FA_example3_triggered();
    void on_RE_FA_example4_triggered();
    void on_RE_FA_example5_triggered();
    void on_RE_FA_example6_triggered();
    void on_RE_FA_example7_triggered();
    void on_RE_FA_example8_triggered();
    void on_RE_FA_example9_triggered();

    //
    // Remove Epsilon
    //
    void prepareRemoveEpsilon(FiniteAutomata FA = FiniteAutomata());

private:
    Ui::MainWindow *ui;


    //old thinks to delete
    QList<QGraphicsItem*> itemsToDelete;
    int statusBarTimeout;
    //void deleteSelected(QGraphicsScene *scene);

    //
    // General variables
    //
    Algorithm::modes mode;
    Conversions activeConversion;


    //
    // regExp to FA
    //
    RegExpToFA* reg_exp_algorithm;
    QWidget* regExpToFA_central_widget;
    RegExpWidget* regExpWidget;
    FA_widget* fa_widget_left;
    FA_widget* fa_widget_center;
    FA_widget* fa_widget_right;
    RegExpWidget* reg_exp_widget;
    void prepareREtoFA_GUI();

    //
    // Remove epsilon
    //
    RemoveEpsilon* remove_epsilon_algorithm;
    QWidget* removeEpsilon_central_widget;
    QTextEdit* remove_epsilon_variables_widget;
    QListWidget* epsilon_closer_list_widget;
    FA_widget* fa_epsilon_widget;
    FA_widget* fa_not_epsilon_widget;
    void prepareRemoveEpsilon_GUI();

};

#endif // MAINWINDOW_H
