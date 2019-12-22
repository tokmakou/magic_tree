#pragma once

#include <unordered_map>

#include "cacheobject.h"
#include "treeitem.h"

namespace magic_tree
{

class StoreCacheObject
{
public:
    StoreCacheObject();

    TreeItemPtr insert(const CacheObjectPtr & object);
    TreeItemPtr getTreeItem(const std::string & key);
    bool remove(const std::string & key);

private:
    using Cache = std::map<std::string, TreeItemPtr>;

    Cache mCache;
};

}
