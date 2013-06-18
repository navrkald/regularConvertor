#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include <finite_machine/diagramscene.h>
#include <QShortcut>

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
private slots:
private:
    Ui::MainWindow *ui;
    DiagramScene *scene1;
    DiagramScene *scene2;
    QList<QGraphicsItem*> itemsToDelete;

    //void deleteSelected(QGraphicsScene *scene);

//    QToolButton *sceneBut;
};

#endif // MAINWINDOW_H
