#ifndef FA_WIDGET_H
#define FA_WIDGET_H

#include <QWidget>
#include <finite_machine/diagramscene.h>
#include "finite_machine/finiteautomata.h"

namespace Ui {
class FA_widget;
}

class FA_widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FA_widget(QWidget *parent = 0);
    ~FA_widget();
    DiagramScene *scene;
    FiniteAutomata* FA;
    QShortcut* deleteShortCut;
private slots:
    void on_MoveNodeBut_clicked();

    void on_AddNodeBut_clicked();

    void on_AddArrowBut_clicked();

    void on_DeleteNodeBut_clicked();

private:
    Ui::FA_widget *ui;
    QPushButton* MoveNodeBut;
    QPushButton* AddNodeBut;
    QPushButton* AddArrowBut;
    QPushButton* DeleteNodeBut;
};

#endif // FA_WIDGET_H
