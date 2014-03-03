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
public slots:
    void testing_slot();
private slots:
    void myStatusbarShowMessage(QString message);
    void prepareREtoFA();
    void on_action_check_mode_triggered();

    void on_action_play_mode_triggered();

    void on_action_step_mode_triggered();

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
    //void deleteSelected(QGraphicsScene *scene);

//    QToolButton *sceneBut;
};

#endif // MAINWINDOW_H
