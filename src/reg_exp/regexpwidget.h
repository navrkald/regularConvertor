#ifndef REGEXPWIDGET_H
#define REGEXPWIDGET_H

#include <QWidget>
#include "reg_exp/regexp.h"
#include "reg_exp/regexptreemodel.h"
#include <QTreeView>

namespace Ui {
class RegExpWidget;
}

class CRegExpWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRegExpWidget(QWidget *parent = 0);
    ~CRegExpWidget();
    RegExp* re;
     QItemSelectionModel *selectionModel;


public:
    Ui::RegExpWidget *ui;
    RegExpTreeModel* treeModel;
    QTreeView* treeView;
    void updateView();
    void deselectAll();
    void setRegExp(RegExp* _re);

signals:
    void newRegExp(RegExp* re);
    void itemClicked(QModelIndex index);

public slots:
    void modelChanged();
    void on_pushButton_clicked();
};

#endif // REGEXPWIDGET_H
