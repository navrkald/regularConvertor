#ifndef FA_WIDGET_H
#define FA_WIDGET_H

#include <QWidget>
#include <QDebug>
#include "diagramscene.h"
#include "finiteautomata.h"
#include <QtAlgorithms>
#include "multiselectcompleter.h"
#include "editruledialog.h"

namespace Ui
{
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
    void MoveNodeBut_clicked();

    void AddNodeBut_clicked();

    void AddArrowBut_clicked();

    void DeleteNodeBut_clicked();

    void statesEdited();
    void endingStatesEdited();
    void alphaberEdited();
    //void on_startStateLineEdit_textChanged(const QString &arg1);

    void on_startStateComboBox_currentIndexChanged(const QString &arg1);

    void on_startStateComboBox_activated(const QString &arg1);


    void on_addRuleToolButton_clicked();

    void on_removeRuleToolButton_clicked();

    void on_rulesListWidget_itemDoubleClicked(QListWidgetItem *item);

public slots:
    void updateStates();

private:
    //properties
    Ui::FA_widget *ui;
    QPushButton* MoveNodeBut;
    QPushButton* AddNodeBut;
    QPushButton* AddArrowBut;
    QPushButton* DeleteNodeBut;
    //validators for qlineedits
    QValidator *statesValidator;
    QValidator *alphabetValidator;
    QRegExpValidator *endingStatesValidator;
    QValidator *rules;
    MultiSelectCompleter *endingStatesCompleter;
    QStringListModel *statesStringListModel;//this is needed for QCompleter
    //methods
    void setupValidators();
    QStringList getSortedUniqueList(QString raw_text);
    QErrorMessage errorMessage;

};

#endif // FA_WIDGET_H
