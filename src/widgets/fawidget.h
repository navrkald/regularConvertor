#ifndef FA_WIDGET_H
#define FA_WIDGET_H

#include <QWidget>
#include <QDebug>
#include "finite_machine/diagramscene.h"
#include "finite_machine/finiteautomata.h"
#include <QtAlgorithms>
#include "finite_machine/multiselectcompleter.h"
#include "finite_machine/editruledialog.h"

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
    DiagramScene *m_scene;
    FiniteAutomata* m_finiteAutomata;
    QShortcut* deleteShortCut;

    void SetCaption(const QString& caption);
    static QStringList getSortedUniqueList(QString raw_text);
    void setCorrectStatus();
    void setWrongStatus();
    void clearStatus();
protected:
    void SetScene(DiagramScene * scene);
    void SetSceneGeneral(DiagramScene * scene);
    virtual void SetSceneSpecific(DiagramScene * scene);

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
    void setFA(FiniteAutomata* m_finiteAutomata);
    void updateStates(); // Update formal FA

    void emitAddEdge(ComputationalRules rule);
    void emitAddEndingNode(QString node);

signals:
    void FA_changed(FiniteAutomata* m_finiteAutomata);
    void setFA_signalToScene(FiniteAutomata* m_finiteAutomata); // update graphic FA
    void addNodes(QSet <QString> nodes);
    void removeNodes(QSet <QString> nodes);
    void addEdges(QSet <ComputationalRules> rules);
    void removeEdges(QSet <ComputationalRules> rules);
    void setStartNode(QString n);
    void addEndingNodes(QSet <QString> nodes);
    void removeEndingNodes(QSet <QString> nodes);
    void SendStatusBarMessage(QString message);

protected:
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
