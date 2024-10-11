#ifndef CACHE_CACHE_H_
#define CACHE_CACHE_H_

#include <functional>

namespace cachelib {

template <typename KeyT, typename T>
class ICache
{
public:
    virtual ~ICache() {}
    virtual bool lookup_update(const KeyT& key, std::function<T(KeyT)> get_content) = 0;
    virtual void dump() const = 0;
    virtual bool is_full() const = 0;
};

}

#endif // CACHE_CACHE_H_
