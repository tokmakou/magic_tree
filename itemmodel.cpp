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

QModelIndex ItemModel::parent(const QModelIndex &child) const
{
    auto childItem(static_cast<TreeItem *>(child.internalPointer()));

    if (childItem == nullptr)
        return QModelIndex();

    auto parentItem(childItem->getParent());
    auto parentInternalPointer(parentItem.get() == &mTree.getRoot()
                               ? nullptr
                                   : parentItem.get());

    if (parentInternalPointer == nullptr)
        return QModelIndex();

    return createIndex(parentInternalPointer->getParent()->childIndex(parentItem),
                       0,
                       parentInternalPointer);
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return mTree.getRoot().getSize();

    auto parentTreeItem(static_cast<TreeItem *>(parent.internalPointer()));
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
    auto row(parentTreeItem ? parentTreeItem->getParent()->childIndex(parentTreeItem) : -1);
    auto parentIndex(parentTreeItem
                         ? createIndex(row, 0, parentTreeItem.get())
                         : QModelIndex());

    beginInsertRows(parentIndex, pos, pos);
}

void ItemModel::endInsert()
{
    endInsertRows();
}

void ItemModel::beginRemove(const TreeItemPtr & parentTreeItem, int pos)
{
    auto row(parentTreeItem ? parentTreeItem->getParent()->childIndex(parentTreeItem) : -1);
    auto parentIndex(parentTreeItem
                         ? createIndex(row, 0, parentTreeItem.get())
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
    auto sourceParentRow(sourceParentTreeItem
                             ? sourceParentTreeItem->getParent()->childIndex(sourceParentTreeItem)
                             : -1);
    auto parentIndex(sourceParentTreeItem
                         ? createIndex(sourceParentRow, 0, sourceParentTreeItem.get())
                         : QModelIndex());

    auto destinationParentRow(destinationParentTreeItem
                                  ? destinationParentTreeItem->getParent()->childIndex(destinationParentTreeItem)
                                  : -1);
    auto destinationIndex(destinationParentTreeItem
                                    ? createIndex(destinationParentRow, 0, destinationParentTreeItem.get())
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
    auto treeItem(parentTreeItem->getChild(pos));
    Q_ASSERT(treeItem);

    auto index(createIndex(pos, 0, treeItem.get()));

    QAbstractItemModel::dataChanged(index, index);
}

Tree &ItemModel::getTree()
{
    return mTree;
}
