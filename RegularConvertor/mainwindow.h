#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include <diagramscene.h>

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
    //void on_graphicsView_customContextMenuRequested(const QPoint &pos);

    void on_AddNodeBut_clicked();

    void on_MoveNodeBut_clicked();

    void on_DeleteNodeBut_clicked();
    void deleteItem();

    void on_AddArrow_clicked(bool checked);
    void delaidDelete();
private:
    Ui::MainWindow *ui;
    DiagramScene *scene1;
    DiagramScene *scene2;
    QList<QGraphicsItem*> itemsToDelete;

    //void deleteSelected(QGraphicsScene *scene);

//    QToolButton *sceneBut;
};

#endif // MAINWINDOW_H
