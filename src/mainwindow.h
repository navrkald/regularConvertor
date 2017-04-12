#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include "finite_machine/diagramscene.h"
#include <QShortcut>
#include "widgets/fawidget.h"
#include "reg_exp/regextextedit.h"
#include "reg_exp/regexpwidget.h"
#include "widgets/algorithmwidget.h"
#include "algorithms/algorithm.h"
#include "algorithms/removeepsilon.h"
#include "algorithms/fatodfa.h"
#include "algorithms/dfatominfa.h"
#include <algorithms/regexptofa.h>
#include <PDA/pdawidget.h>
#include <CFG/cfgwidget.h>
#include <gui_algorithm_interface/cfgtopdaguiinterface.h>
#include <widgets/centralwidgetinterface.h>

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
    enum Conversions {none, RE_to_FA, REMOVE_EPSILON, DFA, CFG_TO_PDA};
    CAlgorithmWidget* alhgorithm_widget;
    QTranslator* translator;

signals:
    void modeChanged(AlgorithmModes mode);
public slots:
    void showStatusMessage(QString message);
    void mySetWindowTitle(QString example_name = "");
private slots:
    void hideStatusMessage();
    void on_action_check_mode_triggered();
    void on_action_play_mode_triggered();
    void on_action_step_mode_triggered();

    void PrepareConversionWidget(Conversions conversion);

    //
    // RegExp to FA
    //
    void RE_FA_example(RegExp* _re, QString example_name = "");
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
    void RemoveEpsilon_example(FiniteAutomata _FA, QString example_name = "");
    void on_RemoveEpsilon_example0_triggered();
    void on_RemoveEpsilon_advanced_example1_triggered();

    //
    // DFA
    //
    void Determinization_example(FiniteAutomata _FA, QString example_name = "");
    void on_Determinization_example_1_triggered();

    void on_Determinization_advanced_example_1_triggered();

    void on_action_save_triggered();

    void on_action_open_file_triggered();

    void on_RemoveEpsilon_example1_triggered();

    void on_RemoveEpsilon_example2_triggered();

    void on_RemoveEpsilon_example3_triggered();

    void on_RemoveEpsilon_advanced_example2_triggered();

    void on_RemoveEpsilon_advanced_example3_triggered();

    void on_RemoveEpsilon_advanced_example4_triggered();

    void on_Determinization_example_2_triggered();

    void on_Determinization_example_3_triggered();

    void on_Determinization_example_4_triggered();

    void on_Determinization_advanced_example_2_triggered();

    void on_Determinization_advanced_example_3_triggered();

    void on_Determinization_advanced_example_4_triggered();

    void on_actionCzech_triggered();

    void on_actionEnglish_triggered();

    //
    // CFG to PDA
    //
    void on_actionCFGtoPDA_triggered();

    void on_action_Determinization_triggered();

    void on_action_RE_to_FA_triggered();

    void on_action_RemoveEpsilon_triggered();

private:
    using QMainWindow::setCentralWidget;
    Ui::MainWindow *ui;
    QTimer* status_timer;
    QLabel* status_label;

    // Add actions in menu to groups in order to be checkable only one of them
    void SetActionsGroups();

    //old thinks to delete
    QList<QGraphicsItem*> itemsToDelete;
    int statusBarTimeout;
    //
    // General variables
    //
    AlgorithmModes mode;
    Conversions m_activeConversion;
    ICentralCoversionWidget* m_centralWidget;

};
    QDataStream& operator>>(QDataStream& in, MainWindow::Conversions& e);
#endif // MAINWINDOW_H
