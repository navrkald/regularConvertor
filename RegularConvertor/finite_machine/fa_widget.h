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

    static QStringList getSortedUniqueList(QString raw_text);
    void setCorrectStatus();
    void setWrongStatus();
    void clearStatus();

private slots:
    //sloty na tlacitka na editaci grafu
    void MoveNodeBut_clicked();
    void AddNodeBut_clicked();
    void AddArrowBut_clicked();

    //sloty na editaci formalniho popisu KA
    void statesEdited();
    void endingStatesEdited();
    void alphabetEdited();
    void on_startStateComboBox_activated(const QString &arg1);
    void on_addRuleToolButton_clicked();
    void on_removeRuleToolButton_clicked();
    void on_rulesListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_tabWidget_currentChanged(int index);
    void delete_items();
    void clean();

public slots:
    void setFA(FiniteAutomata* FA);
    void updateStates(); // Update formal FA

    void emitAddEdge(ComputationalRules rule);
    void emitAddEndingNode(QString node);

signals:
    void FA_changed(FiniteAutomata* FA);
    void setFA_signal(FiniteAutomata* FA); // update graphic FA
    void addNodes(QSet <QString> nodes);
    void removeNodes(QSet <QString> nodes);
    void addEdges(QSet <ComputationalRules> rules);
    void removeEdges(QSet <ComputationalRules> rules);
    void setStartNode(QString n);
    void addEndingNodes(QSet <QString> nodes);
    void removeEndingNodes(QSet <QString> nodes);
    void sendStatusBarMessage(QString message);

private:
    //properties
    Ui::FA_widget *ui;
    QLabel* status_label;
    QToolButton* MoveNodeBut;
    QToolButton* AddNodeBut;
    QToolButton* AddArrowBut;
    QToolButton* DeleteNodeBut;
    //validators for qlineedits
    QValidator *statesValidator;
    QValidator *alphabetValidator;
    QRegExpValidator *endingStatesValidator;
    QValidator *rules;
    MultiSelectCompleter *endingStatesCompleter;
    QStringListModel *statesStringListModel;//this is needed for QCompleter
    //methods
    void setupValidators();
};

#endif // FA_WIDGET_H
