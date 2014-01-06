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

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        CharPos charter) const;

private:
    RegExpNode *nodeFromIndex(const QModelIndex &index) const;

    RegExpNode *rootNode;
};

#endif // REGEXPTREEMODEL_H
