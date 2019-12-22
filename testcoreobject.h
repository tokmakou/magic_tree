#pragma once

#include "tree/cacheobject.h"

class TestCoreObject : public magic_tree::CacheObject
{
public:
    TestCoreObject(const std::string & name);

    // CacheObject interface
public:
    std::string name() const { return mName; }
    QVariant data(int role) const { return QString::fromStdString(mName); };

    std::string mName;
};
