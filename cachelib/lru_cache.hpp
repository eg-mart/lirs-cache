#ifndef CACHE_LRU_CACHE_H_
#define CACHE_LRU_CACHE_H_

#include <iostream>
#include <unordered_map>
#include <utility>
#include <list>
#include <functional>

namespace cachelib {

template <typename KeyT, typename T>
class LRUCache
{
private:
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;
    std::list<std::pair<KeyT, T>> cache_;
    size_t size_;


public:
    LRUCache(size_t size) : size_(size) {};

    void dump(std::ostream& os) const
    {
        for (auto& e : cache_)
            os << e.second << " ";
        os << std::endl;
    }

    bool is_full() const
    {
        return cache_.size() >= size_;
    }

    bool lookup_update(const KeyT& key, std::function<T(KeyT)> get_content)
    {
        HashIt cached_elem = hash_.find(key);
        if (cached_elem != hash_.end()) {
            auto cached_elem_it = cached_elem->second;
            if (cached_elem_it != cache_.begin())
                cache_.splice(cache_.begin(), cache_, cached_elem_it,
                              std::next(cached_elem_it));
            return true;
        }

        if (is_full()) {
            hash_.erase(cache_.back().first);
            cache_.pop_back();
        }

        cache_.emplace(key, get_content(key));
        hash_[key] = cache_.begin();

        return false;
    }
};

}

#endif // CACHE_LRU_CACHE_H_
