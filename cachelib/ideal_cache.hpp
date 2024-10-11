#ifndef CACHE_IDEAL_CACHE_H_
#define CACHE_IDEAL_CACHE_H_

#include <iostream>
#include <unordered_map>
#include <deque>
#include <list>
#include <functional>
#include <cassert>

#include "cache.hpp"

namespace cachelib {

template <typename KeyT, typename T>
class IdealCache : public ICache<KeyT, T>
{
private:
    struct Block
    {
        KeyT key;
        T content;
        std::deque<size_t>& req_queue;
    };

    using ConstListIt = typename std::list<Block>::const_iterator;
    using HashIt = typename std::unordered_map<KeyT, ConstListIt>::iterator;
    using ReqIt  = typename std::unordered_map<KeyT, std::deque<size_t>>::iterator;

    std::unordered_map<KeyT, std::deque<size_t>> requests_;
    std::unordered_map<KeyT, ConstListIt> hash_;
    std::list<Block> cache_;
    size_t size_;


public:
    template <typename Iterator>
    IdealCache(size_t size, Iterator start, Iterator end) : size_(size)
    {
        size_t ind = 0;
        for (Iterator it = start; it != end; it++) {
            requests_[*it].push_back(ind);
            ind++;
        }
    }

    void dump(std::ostream& os) const
    {
        for (auto& e : cache_) {
            os << "(" << e.key << ") ";
            for (auto q : e.req_queue)
                os << q << " ";
            os << "\n";
        }
    }

    bool is_full() const
    {
        return cache_.size() >= size_;
    }

    bool lookup_update(const KeyT& key, std::function<T(KeyT)> get_content)
    {
        ReqIt elem_reqs = requests_.find(key);
        elem_reqs->second.pop_front();

        HashIt cached_elem = hash_.find(key);
        if (cached_elem != hash_.end())
            return true;

        if (elem_reqs->second.empty())
            return false;

        Block new_elem = {.key=key, .content=get_content(key),
                          .req_queue=elem_reqs->second};

        if (!is_full()) {
            cache_.push_front(new_elem);
            hash_[key] = cache_.begin();
            return false;
        }

        ConstListIt furthest_elem = get_furthest_element_();
        assert(("furthest_elem iterator isn't valid\n", furthest_elem != cache_.cend()));

        if (furthest_elem->req_queue.empty() ||
            furthest_elem->req_queue.front() > elem_reqs->second.front()) {        
            hash_.erase(furthest_elem->key);
            cache_.erase(furthest_elem);
            cache_.push_front(new_elem);
            hash_[key] = cache_.begin();
        }

        return false;
    }

private:
    ConstListIt get_furthest_element_() const
    {
        ConstListIt cur_max = cache_.cend();
        size_t cur_max_ind = -1;

        for (ConstListIt it = cache_.cbegin(); it != cache_.cend(); it++) {
            if (it->req_queue.empty()) {
                cur_max = it;
                break;
            }

            if (cur_max_ind < it->req_queue.front() || cur_max_ind == -1) {
                cur_max_ind = it->req_queue.front();
                cur_max = it;
            }
        }

        return cur_max;
    }
};

}

#endif // CACHE_IDEAL_CACHE_H_
