#include "treeitem.h"

using namespace magic_tree;

TreeItem::TreeItem(const CacheObjectPtr & cacheObject)
    : mCacheObject(cacheObject)
    , mParent(nullptr)
{

}

int TreeItem::childCount()
{
    return mChilds.size();
}

int TreeItem::childIndex(const TreeItemPtr & child) const
{
    auto childIterator(std::find(mChilds.cbegin(), mChilds.cend(), child));
    Q_ASSERT(childIterator != mChilds.end());

    return std::distance(mChilds.begin(), childIterator);
}

void TreeItem::addChild(const TreeItemPtr & child)
{
    Q_ASSERT(child->getParent().get() == this);

    mChilds.push_back(child);
}

void TreeItem::removeChild(const TreeItemPtr & child)
{
    auto childIterator(std::find(mChilds.begin(), mChilds.end(), child));
    Q_ASSERT(childIterator != mChilds.end());

    mChilds.erase(childIterator);
}

const TreeItemChilds & TreeItem::getChilds()
{
    return mChilds;
}

std::size_t TreeItem::getSize() const
{
    return mChilds.size();
}

TreeItemPtr TreeItem::getChild(std::size_t index) const
{
    Q_ASSERT(index < mChilds.size());
    Q_ASSERT(!mChilds.empty());

    return *std::next(mChilds.begin(), index);
}

TreeItemPtr TreeItem::getParent() const
{
    return mParent;
}

void TreeItem::setParent(const TreeItemPtr &parent)
{
    mParent = parent;
}

std::string TreeItem::getKey() const
{
    return mCacheObject->key();
}

void TreeItem::resetCacheObject(const CacheObjectPtr &cacheObject)
{
    mCacheObject = cacheObject;
}

const CacheObject &TreeItem::getCacheObject() const
{
    return *mCacheObject.get();
}

int64_t TreeItem::row() const
{
    Q_ASSERT(mParent != nullptr);

    auto iterator(std::find_if(mParent->mChilds.cbegin(), mParent->mChilds.cend(),
                                 [this](const TreeItemPtr & child)
                                 {
                                   return child.get() == this;
                                 }));

    Q_ASSERT(iterator != mParent->mChilds.end());

    return std::distance(mParent->mChilds.cbegin(), iterator);
}

