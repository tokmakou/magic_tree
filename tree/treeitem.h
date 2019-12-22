#pragma once

#include <memory>
#include <list>
#include <string>

#include "cacheobject.h"

namespace magic_tree
{

class TreeItem;
using TreeItemPtr = std::shared_ptr<TreeItem>;
using TreeItemChilds = std::list <TreeItemPtr>;

class TreeItem
{
public:
    TreeItem(const CacheObjectPtr & cacheObject);

    int childCount();;
    int childIndex(const TreeItemPtr & child) const;

    void addChild(const TreeItemPtr & child);
    void removeChild(const TreeItemPtr & child);
    const TreeItemChilds & getCilds();
    std::size_t getSize() const;

    TreeItemPtr getChild(std::size_t index) const;

    TreeItemPtr getParent() const;
    void setParent(const TreeItemPtr & parent);

    std::string getKey() const;
    void resetCacheObject(const CacheObjectPtr & cacheObject);

    const CacheObject & getCacheObject() const;

private:
    CacheObjectPtr mCacheObject;
    TreeItemPtr mParent;
    TreeItemChilds mChilds;
};

}
