#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include "finite_machine/diagramscene.h"
#include <QShortcut>
#include "widgets/fa_widget.h"
#include "reg_exp/regextextedit.h"
#include "reg_exp/regexpwidget.h"
#include "algorithms/regexptofa.h"
#include "widgets/algorithmwidget.h"
#include "algorithms/algorithm.h"
#include "algorithms/removeepsilon.h"
#include "algorithms/fatodfa.h"
#include "algorithms/dfatominfa.h"
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
    void modeChanged(CAlgorithm::modes mode);
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
    void prepareREtoFA(RegExp* _re = new RegExp());
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
    void prepareRemoveEpsilon();
    void RemoveEpsilon_example(FiniteAutomata _FA, QString example_name = "");
    void on_RemoveEpsilon_example0_triggered();
    void on_RemoveEpsilon_advanced_example1_triggered();

    //
    // DFA
    //
    void PrepareDFA();
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
    void PrepareCFGtoPDA();

    void on_actionCFGtoPDA_triggered();

    void on_action_Determinization_triggered();

private:
    using QMainWindow::setCentralWidget;
    Ui::MainWindow *ui;
    QTimer* status_timer;
    QLabel* status_label;

    // Add actions in menu to groups in order to be checkable only one of them
    void SetActionsGroups();

    // General private function
    QWidget* prepareAlgorithnContainer(QWidget* central_w, QString str_label, CAlgorithm* algorithm);
    QWidget* prepareFAContainer(QWidget* central_w, QString str_label, FA_widget* fa_widget);
    QWidget* variablesContainer(QWidget* central_w, QString str_label, QLabel* var_widget);
    QWidget* horizontalContainer(QWidget* central_w, QList<QWidget*> widgets);
    QWidget* verticalContainer(QWidget* central_w, QList<QWidget*> widgets);

    //old thinks to delete
    QList<QGraphicsItem*> itemsToDelete;
    int statusBarTimeout;
    //void deleteSelected(QGraphicsScene *scene);

    //
    // General variables
    //
    CAlgorithm::modes mode;
    Conversions m_activeConversion;
    ICentralCoversionWidget* m_centralWidget;


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
    QLabel* remove_epsilon_variables_widget;
    QListWidget* epsilon_closer_list_widget;
    FA_widget* fa_epsilon_widget;
    FA_widget* fa_not_epsilon_widget;
    void prepareRemoveEpsilon_GUI();

    //
    // Determinization
    //
    FaToDFA* DFA_algorithm;
    QWidget* DFA_central_widget;
    QLabel* DFA_variables_widget;
    FA_widget* not_DFA_widget;
    FA_widget* DFA_widget;
    void PrepareDFA_GUI();

    //
    // CFG to PDA
    //
    CCfgToPdaGuiInterface* CFG_TO_PDA_algorithm;
    QWidget* CFG_TO_PDA_central_widget;
    QLabel* CFG_TO_PDA_variables_widget;
    CPdaWidget* m_pdaWidget;
    CCfgWidget* m_cfgWidget;
    void prepareCFG_TO_PDA_GUI();


};
    QDataStream& operator>>(QDataStream& in, MainWindow::Conversions& e);
#endif // MAINWINDOW_H
