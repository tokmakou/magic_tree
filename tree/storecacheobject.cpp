#include "storecacheobject.h"

using namespace magic_tree;

StoreCacheObject::StoreCacheObject()
{

}

TreeItemPtr StoreCacheObject::getTreeItem(const std::string & key)
{
    auto treeItem(mCache.find(key));

    if (treeItem == mCache.end())
        return TreeItemPtr();

    return treeItem->second;
}

TreeItemPtr StoreCacheObject::insert(const CacheObjectPtr & object)
{
    const auto & key(object->key());
    auto treeItemIter(mCache.find(key));

    if (treeItemIter == mCache.end())
    {
        auto treeItem(std::make_shared<TreeItem>(object));
        auto treeItemIter = mCache.insert(std::make_pair(key, treeItem)).first;
        return treeItemIter->second;
    }

    treeItemIter->second->resetCacheObject(object);
    return treeItemIter->second;
}

bool StoreCacheObject::remove(const std::string & key)
{
    auto treeItem(mCache.find(key));

    if (treeItem == mCache.end())
        return false;

    mCache.erase(key);
    return true;
}
