#ifndef REGEXPTREEMODEL_H
#define REGEXPTREEMODEL_H

#include <QAbstractItemModel>
#include "regexpnode.h"
#include "charpos.h"

//class Node;

class RegExpTreeModel : public QAbstractItemModel
{
public:
    RegExpTreeModel(QObject *parent = 0);
    ~RegExpTreeModel();

    void setRootNode(RegExpNode *node);

    //vrati index ktery mam row colum a rodice
    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    //vrati rodice zadaneho indexu
    QModelIndex parent(const QModelIndex &child) const;

    //vrati pocet radku od rodice
    int rowCount(const QModelIndex &parent) const;
    //vrati pocet sloupcu od rodice
    int columnCount(const QModelIndex &parent) const;
    //vrati data indexu se specifickou rolý (text a ikonu)
    QVariant data(const QModelIndex &index, int role) const;

    //vrati
    QModelIndex indexFromNode(RegExpNode *node) const;
    //vrati data na indexu
    RegExpNode *nodeFromIndex(const QModelIndex &index) const;
private:


    RegExpNode *rootNode;
};

#endif // REGEXPTREEMODEL_H
