#include "itemmodel.h"

using namespace magic_tree;

ItemModel::ItemModel(QObject *parent)
    : QAbstractItemModel(parent)
    , mTree(*this)
{

}

QModelIndex ItemModel::index(int row, int column, const QModelIndex & parent) const
{
    const auto parentItem(parent.isValid()
                          ? static_cast<TreeItem *>(parent.internalPointer())
                          : &mTree.getRoot());

    return createIndex(row, column, parentItem->getChild(row).get());
}

QModelIndex ItemModel::parent(const QModelIndex & child) const
{
    auto childItem(static_cast<TreeItem *>(child.internalPointer()));

    if (childItem == nullptr)
        return QModelIndex();

    auto parentItem(childItem->getParent());

    if (parentItem.get() == & mTree.getRoot())
        return QModelIndex();

    auto parentIndex(parentItem->row());

    return createIndex(parentIndex, 0, parentItem.get());
}

int ItemModel::rowCount(const QModelIndex & parent) const
{   
    auto parentTreeItem(parent.isValid()
                        ? static_cast<TreeItem *>(parent.internalPointer())
                            : & mTree.getRoot());

    return parentTreeItem->getSize();
}

int ItemModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto treeItem(static_cast<TreeItem *>(index.internalPointer()));

    return treeItem->getCacheObject().data(role);
}

void ItemModel::beginInsert(const TreeItemPtr & parentTreeItem, int pos)
{
    auto parentIndex(parentTreeItem
                         ? createIndex(parentTreeItem->row(), 0, parentTreeItem.get())
                         : QModelIndex());

    beginInsertRows(parentIndex, pos, pos);
}

void ItemModel::endInsert()
{
    endInsertRows();
}

void ItemModel::beginRemove(const TreeItemPtr & parentTreeItem, int pos)
{
    auto parentIndex(parentTreeItem
                         ? createIndex(parentTreeItem->row(), 0, parentTreeItem.get())
                         : QModelIndex());

    beginRemoveRows(parentIndex, pos, pos);
}

void ItemModel::endRemove()
{
    endRemoveRows();
}

void ItemModel::beginMove(const TreeItemPtr & sourceParentTreeItem, int sourcePos,
                          const TreeItemPtr & destinationParentTreeItem, int pos)
{
    auto parentIndex(sourceParentTreeItem
                         ? createIndex(sourceParentTreeItem->row(), 0, sourceParentTreeItem.get())
                         : QModelIndex());

    auto destinationIndex(destinationParentTreeItem
                                    ? createIndex(destinationParentTreeItem->row(), 0, destinationParentTreeItem.get())
                                    : QModelIndex());

    beginMoveRows(parentIndex, sourcePos, sourcePos,
                  destinationIndex, pos);
}

void ItemModel::endMove()
{
    endMoveRows();
}

void ItemModel::dataChanged(const TreeItemPtr & parentTreeItem, int pos)
{
    auto index(parentTreeItem
                   ? createIndex(pos, 0, parentTreeItem->getChild(pos).get())
                   : createIndex(pos, 0, mTree.getRoot().getChild(pos).get()));

    QAbstractItemModel::dataChanged(index, index);
}

Tree &ItemModel::getTree()
{
    return mTree;
}
