#pragma once

#include <memory>
#include <QAbstractItemModel>

#include "tree/tree.h"

namespace magic_tree
{

class ItemModel : public QAbstractItemModel, public magic_tree::ITreeChangesListener
{
public:
    ItemModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    // ITreeChangesListener interface
public:
    void beginInsert(const TreeItemPtr &parentTreeItem, int pos);
    void endInsert();
    void beginRemove(const TreeItemPtr &parentTreeItem, int pos);
    void endRemove();
    void beginMove(const TreeItemPtr &sourceParentTreeItem, int sourcePos,
                   const TreeItemPtr &destinationParentTreeItem, int pos);
    void endMove();
    void dataChanged(const TreeItemPtr &parentTreeItem, int pos);

public:
    Tree & getTree();

private:
    Tree mTree;
};

} // magic_tree namespace
