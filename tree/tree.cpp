#include "tree.h"

using namespace magic_tree;

namespace
{

class RootCacheObject : public CacheObject
{
    // CacheObject interface
public:
    std::string key() const override { return std::string(); }
    QVariant data(int) const override { return QVariant(); }
};

CacheObjectPtr makeRootCacheObject() { return CacheObjectPtr(new RootCacheObject()); }
TreeItemPtr makeRootTreeItem() { return TreeItemPtr(new TreeItem(makeRootCacheObject())); }

} // anonymous namespace

Tree::Tree(ITreeChangesListener & treeChangesListener)
    : mRoot(TreeItemPtr(makeRootTreeItem()))
    , mCache(new StoreCacheObject())
    , mChangesListener(treeChangesListener)
{

}

void Tree::insert(const CacheObjectPtr &object)
{
    auto insertedItem(mCache->insert(object));

    if (insertedItem->getParent() == nullptr)
    {
        insert(insertedItem);
        return;
    }

    const auto insertedItemParent(insertedItem->getParent());
    const auto insertedPos(insertedItem->row());
    mChangesListener.dataChanged(getModelParentPtr(insertedItem), insertedPos);
}

void Tree::move(const std::string & key, const std::string & parent_key)
{
    auto treeItem(mCache->getTreeItem(key));
    auto newParent(mCache->getTreeItem(parent_key));

    if (!treeItem || !newParent)
        return;

    move(treeItem, newParent);
}

void Tree::moveToRootIfThisParent(const std::string &key, const std::string &parent_key)
{
    auto treeItem(mCache->getTreeItem(key));
    auto parent(mCache->getTreeItem(parent_key));

    if (!treeItem)
        return;

    if (parent == treeItem->getParent())
        move(treeItem, mRoot);
}

void Tree::moveToRoot(const std::string & key)
{
    auto treeItem(mCache->getTreeItem(key));

    if (!treeItem)
        return;

    move(treeItem, mRoot);
}

void Tree::remove(const std::string & key)
{
    auto treeItem(mCache->getTreeItem(key));

    if (!treeItem)
        return;

    const auto & childList(treeItem->getChilds());

    while (!childList.empty()) {
        move(*childList.begin(), mRoot);
    }

    remove(treeItem);
}

const TreeItem & Tree::getRoot() const
{
    return *mRoot.get();
}

void Tree::move(const TreeItemPtr & item, const TreeItemPtr & parent)
{
    const auto currentParent(item->getParent());

    if (currentParent == parent)
        return;

    const auto currentPos(item->row());

    mChangesListener.beginMove(getModelParentPtr(item), currentPos,
                               parent == mRoot ? nullptr : parent, parent->childCount());

    currentParent->removeChild(item);
    item->setParent(parent);
    parent->addChild(item);

    mChangesListener.endMove();
}

void Tree::remove(const TreeItemPtr & item)
{
    Q_ASSERT(item != mRoot);

    const auto parent(item->getParent());
    const auto pos(item->row());

    mChangesListener.beginRemove(getModelParentPtr(item), pos);

    parent->removeChild(item);
    mCache->remove(item->getKey());

    mChangesListener.endRemove();
}

void Tree::insert(const TreeItemPtr &item)
{
    Q_ASSERT(item != mRoot);

    mChangesListener.beginInsert(nullptr, mRoot->getSize());

    item->setParent(mRoot);
    mRoot->addChild(item);

    mChangesListener.endInsert();
}

TreeItemPtr Tree::getModelParentPtr(const TreeItemPtr & item)
{
    Q_ASSERT(item != mRoot);
    Q_ASSERT(item);
    return item->getParent() == mRoot ? nullptr : item->getParent();
}
