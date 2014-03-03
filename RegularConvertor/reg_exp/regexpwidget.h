#ifndef REGEXPWIDGET_H
#define REGEXPWIDGET_H

#include <QWidget>
#include "reg_exp/regexp.h"
#include "reg_exp/regexptreemodel.h"

namespace Ui {
class RegExpWidget;
}

class RegExpWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RegExpWidget(QWidget *parent = 0);
    ~RegExpWidget();
    RegExp* re;
    
private:
    Ui::RegExpWidget *ui;
    RegExpTreeModel* treeModel;
private slots:
    void modelChanged();
    void on_pushButton_clicked();
};

#endif // REGEXPWIDGET_H
