#include "tree.h"

using namespace magic_tree;

namespace
{

class RootCacheObject : public CacheObject
{
    // CacheObject interface
public:
    std::string name() const { return std::string(); }
    QVariant data(int) const { return QVariant(); }
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
        move(insertedItem, mRoot);
        return;
    }

    const auto insertedItemParent(insertedItem->getParent());
    const auto insertedPos(insertedItemParent->childIndex(insertedItem));
    mChangesListener.dataChanged(getModelParentPtr(insertedItemParent), insertedPos);
}

void Tree::move(const std::string & key, const std::string & parent_key)
{
    auto treeItem(mCache->getTreeItem(key));
    auto newParent(mCache->getTreeItem(parent_key));

    Q_ASSERT(treeItem && newParent);

    move(treeItem, newParent);
}

void Tree::moveToRoot(const std::string & key)
{
    auto treeItem(mCache->getTreeItem(key));
    Q_ASSERT(treeItem);

    move(treeItem, mRoot);
}

void Tree::remove(const std::string & key)
{
    auto treeItem(mCache->getTreeItem(key));
    Q_ASSERT(treeItem);

    const auto & childList(treeItem->getCilds());

    for(const auto & child : childList)
    {
        move(child, mRoot);
    }

    remove(treeItem);
}

const TreeItem & Tree::getRoot() const
{
    return *mRoot.get();
}

void Tree::move(const TreeItemPtr & item, const TreeItemPtr & parent)
{
    Q_ASSERT(item != mRoot);

    const auto currentParent(item->getParent());
    const auto currentPos(currentParent->childIndex(item));

    mChangesListener.beginMove(getModelParentPtr(currentParent), currentPos,
                               getModelParentPtr(parent), parent->childCount());

    currentParent->removeChild(item);
    item->setParent(parent);
    parent->addChild(item);

    mChangesListener.endMove();
}

void Tree::remove(const TreeItemPtr & item)
{
    Q_ASSERT(item != mRoot);

    const auto parent(item->getParent());
    const auto pos(parent->childIndex(item));

    mChangesListener.beginRemove(getModelParentPtr(parent), pos);

    parent->removeChild(item);
    mCache->remove(item->getKey());

    mChangesListener.endRemove();
}

TreeItemPtr Tree::getModelParentPtr(const TreeItemPtr & item)
{
    return item == mRoot ? mModelRoot : item->getParent();
}
