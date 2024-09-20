#ifndef CACHE_IDEAL_CACHE_H_
#define CACHE_IDEAL_CACHE_H_

#include <iostream>
#include <unordered_map>
#include <queue>
#include <list>
#include <vector>

#include "cache.hpp"

namespace cachelib {

template <typename KeyT, typename T, typename F>
class IdealCache : public ICache<KeyT, T, F>
{
private:
    struct Block
    {
        KeyT key;
        T content;
        std::queue<int>& req_queue;
    };

    using ListIt = typename std::list<Block>::iterator;
    using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;
    using ReqIt  = typename std::unordered_map<KeyT, std::queue<int>>::iterator;

    std::unordered_map<KeyT, std::queue<int>> requests_;
    std::unordered_map<KeyT, ListIt> hash_;
    std::list<Block> cache_;
    size_t size_;


public:
    IdealCache(size_t size, std::vector<int> reqs) : size_(size)
    {
        for (int i = 0; i < reqs.size(); i++)
            requests_[reqs[i]].push(i);
    };

    void dump()
    {
        for (auto e : cache_) {
            std::cout << "(" << e.key << ") ";
            std::queue<int> queue_copy = e.req_queue;
            while (!queue_copy.empty()) {
                std::cout << queue_copy.front() << " ";
                queue_copy.pop();
            }
            std::cout << std::endl;
        }
    }

    bool is_full()
    {
        return cache_.size() >= size_;
    }

    bool lookup_update(KeyT key, F get_content)
    {
        ReqIt elem_reqs = requests_.find(key);
        elem_reqs->second.pop();

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

        ListIt furthest_elem = get_furthest_element_();

        if (furthest_elem->req_queue.empty() ||
            furthest_elem->req_queue.front() > elem_reqs->second.front()) {        
            cache_.erase(furthest_elem);
            hash_.erase(furthest_elem->key);
            cache_.push_front(new_elem);
            hash_[key] = cache_.begin();
        }

        return false;
    }

private:
    ListIt get_furthest_element_()
    {
        ListIt cur_max = cache_.end();
        int cur_max_ind = -1;

        for (ListIt it = cache_.begin(); it != cache_.end(); it++) {
            if (it->req_queue.empty()) {
                cur_max = it;
                break;
            }

            if (cur_max_ind < it->req_queue.front()) {
                cur_max_ind = it->req_queue.front();
                cur_max = it;
            }
        }

        return cur_max;
    }
};

}

#endif // CACHE_IDEAL_CACHE_H_
