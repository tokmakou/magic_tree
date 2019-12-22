#pragma once

#include <string>

#include "treeitem.h"
#include "storecacheobject.h"

namespace magic_tree
{

class ITreeChangesListener
{
public:
    virtual ~ITreeChangesListener() = default;

    virtual void beginInsert(const TreeItemPtr & parentTreeItem, int pos) = 0;
    virtual void endInsert() = 0;

    virtual void beginRemove(const TreeItemPtr & parentTreeItem, int pos) = 0;
    virtual void endRemove() = 0;

    virtual void beginMove(const TreeItemPtr & sourceParentTreeItem, int sourcePos,
                           const TreeItemPtr & destinationParentTreeItem, int pos) = 0;
    virtual void endMove() = 0;

    virtual void dataChanged(const TreeItemPtr & parentTreeItem, int pos) = 0;
};

class Tree
{
public:
    Tree(ITreeChangesListener & treeChangesListener);

    void insert(const CacheObjectPtr & object);

    void move(const std::string & key, const std::string & parent_key);
    void moveToRoot(const std::string & key);
    void remove(const std::string & key);

    const TreeItem & getRoot() const;

private:
    void move(const TreeItemPtr & item, const TreeItemPtr & parent);
    void remove(const TreeItemPtr & item);
    void insert(const TreeItemPtr & item);

    TreeItemPtr getModelParentPtr(const TreeItemPtr & item);

private:
    TreeItemPtr mRoot;

    std::unique_ptr<StoreCacheObject> mCache;

    ITreeChangesListener & mChangesListener;
};

using TreeUPtr = std::unique_ptr<Tree>;

}
