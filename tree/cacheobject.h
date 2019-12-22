#pragma once

#include <string>
#include <memory>
#include <QVariant>

namespace magic_tree
{

class CacheObject
{
public:
    CacheObject();
    virtual ~CacheObject();

    virtual std::string key() const = 0;
    virtual QVariant data(int role) const = 0;
};

using CacheObjectPtr = std::shared_ptr<CacheObject>;

}
