#include "regexptreemodel.h"
#include <QtCore>
#include <QAbstractItemModel>
#include <QtGui>

RegExpTreeModel::RegExpTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootNode = 0;
}

RegExpTreeModel::~RegExpTreeModel()
{
    delete rootNode;
}

void RegExpTreeModel::setRootNode(RegExpNode *node)
{
    beginResetModel();
    delete rootNode;
    CharPos emptyCharter("",-1,false);
    RegExpNode* emptyNode = new RegExpNode(emptyCharter);
    //RegExpNode* copy_node = new RegExpNode(node);
    if(node != 0)
    {
        emptyNode->children.append(node);
        node->parent = emptyNode;
    }
    rootNode = emptyNode;
    endResetModel();
}

QModelIndex RegExpTreeModel::index(int row, int column,
                                const QModelIndex &parent) const
{
    if (!rootNode || row < 0 || column < 0)
        return QModelIndex();
    RegExpNode *parentNode = nodeFromIndex(parent);
    RegExpNode *childNode = parentNode->children.value(row);
    if (!childNode)
        return QModelIndex();
    return createIndex(row, column, childNode);
}

QModelIndex RegExpTreeModel::parent(const QModelIndex &child) const
{
    RegExpNode *node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();

    RegExpNode *parentNode = node->parent;
    if (!parentNode)
        return QModelIndex();

//    RegExpNode *grandparentNode = parentNode->parent;
//    if (!grandparentNode)
//        return createIndex(2, 0, parentNode);

    int row = parentNode->children.indexOf(node);
    return createIndex(row, 0, parentNode);
}

int RegExpTreeModel::rowCount(const QModelIndex &parent) const
{
    //if (parent.column() > 0)
    //    return 0;
    RegExpNode *parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return 0;
    return parentNode->children.count();
}

int RegExpTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

QVariant RegExpTreeModel::data(const QModelIndex &index, int role) const
{
    RegExpNode *node = nodeFromIndex(index);
    if (!node)
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        QIcon icon;
        switch( node->state )
        {
            case RegExpNode::CORRECT:
                icon.addFile(":/reg_exp/reg_exp/pictures/ok.png");
                break;
            case RegExpNode::WRONG:
                icon.addFile(":/reg_exp/reg_exp/pictures/wrong.png");
                break;
            case RegExpNode::UNKNOWN:
                icon.addFile(":/reg_exp/reg_exp/pictures/warning.png");
            break;
        //case default:


        }
        return icon;
    }


    if (role == Qt::DisplayRole)
        return node->symbol.charter;;


    if((role == Qt::FontRole))
    {
        return QVariant();
    }
    else
        return QVariant();
}

QVariant RegExpTreeModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  CharPos charter) const
{
//    if (orientation == Qt::Horizontal) {
//        if (section == 0) {
//            return tr("Node");
//        } else if (section == 1) {
//            return tr("Value");
//        }
//    }
    return QVariant();
}

RegExpNode* RegExpTreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        RegExpNode * return_node = static_cast<RegExpNode *>(index.internalPointer());
        return return_node;
    } else {
        return rootNode;
    }
}

QModelIndex RegExpTreeModel::indexFromNode(RegExpNode *node) const//   int row, int column,                                const QModelIndex &parent) const
{
    QList <QModelIndex> indexesToProcese;
    QModelIndex rootIndex = index(0,0,QModelIndex());
    indexesToProcese.append(rootIndex);

    //rowCount(rootIndex);
    //columnCount(rootIndex);

    qDebug() << "Seacherd node" << node->str;

    while(!indexesToProcese.empty())
    {
        QModelIndex processedIndex = indexesToProcese.first();
        indexesToProcese.removeFirst();
        RegExpNode *processedNode = nodeFromIndex(processedIndex);


        if(processedNode != node )
        {
            for(int row = 0; row < rowCount(rootIndex); row++)
            {
                int column = 0;
                indexesToProcese.append(index(row,column,processedIndex));
            }
        }
        else
        {
            qDebug() << "Processed node" << processedNode->str;
            return processedIndex;
        }
    }
    //not fount so return empty index
    qDebug() << "NOT FIND!!!!!";
    return QModelIndex();
}
