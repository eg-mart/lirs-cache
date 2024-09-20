#ifndef CACHE_CACHE_H_
#define CACHE_CACHE_H_

namespace cachelib {

template <typename KeyT, typename T, typename F>
class ICache
{
public:
    virtual ~ICache() {}
    virtual bool lookup_update(KeyT key, F get_content) = 0;
    virtual void dump() = 0;
};

}

#endif // CACHE_CACHE_H_
