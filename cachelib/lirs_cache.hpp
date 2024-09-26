#ifndef CACHE_LIRS_CACHE_H_
#define CACHE_LIRS_CACHE_H_

#include <iostream>
#include <unordered_map>
#include <list>
#include <utility>
#include <functional>

#include "cache.hpp"

namespace cachelib {

template <typename KeyT, typename T>
class LIRSCache : public ICache<KeyT, T>
{
private:

    enum BlockStatus
    {
        LIR,
        HIR_resident,
        HIR_nonresident
    };

    using ContentIt = typename std::list<T>::iterator;

    struct Block
    {
        KeyT key;
        BlockStatus status;
        ContentIt content;
    };

    using BlockIt = typename std::list<Block>::iterator;
    using HashIt  = typename std::unordered_map<KeyT, BlockIt>::iterator;

    std::unordered_map<KeyT, BlockIt> hash_;
    std::list<Block> hirs_;
    std::list<Block> stack_;
    std::list<T>     cache_;
    size_t size_hirs_;
    size_t size_lirs_;


public:
    LIRSCache(size_t size_lirs, size_t size_hirs)
        : size_lirs_(size_lirs)
        , size_hirs_(size_hirs)
    {};

    void dump()
    {
        std::cout << "STACK: ";
        for (auto e : stack_) {
            std::cout << e.key;
            switch (e.status) {
                case BlockStatus::LIR:
                    std::cout << "(LIR)";
                    break;
                case BlockStatus::HIR_resident:
                    std::cout << "(res HIR)";
                    break;
                case BlockStatus::HIR_nonresident:
                    std::cout << "(nonres HIR)";
                    break;
                default:
                    std::cout << "(UNDEF STAT)";
            }
            std::cout << " ";
        }
            
        std::cout << std::endl << "HIRS: ";
        for (auto e : hirs_)
            std::cout << e.key << " ";

        std::cout << std::endl << "CONTENTS: ";
        for (auto e : cache_)
            std::cout << e << " ";

        std::cout << std::endl;
    }

    bool lookup_update(KeyT key, std::function<T(KeyT)> get_content)
    {
        HashIt req_elem_it = hash_.find(key);
        
        if (req_elem_it != hash_.end()) {
            return update_on_stack_hit_(req_elem_it->second, get_content);
        }

        for (auto elem_it = hirs_.begin(); elem_it != hirs_.end(); elem_it++) {
            if (elem_it->key == key) {
                update_on_hirs_hit_(elem_it);
                return true;
            }
        }

        BlockIt new_entry_it = update_on_miss_(key, get_content);
        return false;
    }

    int get_size()
    {
        return cache_.size();
    }

    bool is_cache_full()
    {
        return cache_.size() >= size_lirs_ + size_hirs_;
    }

private:
    bool update_on_stack_hit_(BlockIt hit, std::function<T(KeyT)> get_content)
    {
        bool is_last = (stack_.end() == hit);
        switch (hit->status) {
            case BlockStatus::LIR:
                stack_.splice(stack_.begin(), stack_, hit);
                prune_stack_();
                return true; 

            case BlockStatus::HIR_resident:
                stack_.splice(stack_.begin(), stack_, hit);
                hit->status = BlockStatus::LIR;
                remove_resident_hir_(hit->key);

                if (!stack_.empty()) {
                    stack_.back().status = BlockStatus::HIR_resident;
                    hash_.erase(stack_.back().key);
                    hirs_.splice(hirs_.begin(), stack_, --stack_.end());
                    prune_stack_();
                }
                return true;

            case BlockStatus::HIR_nonresident:
                if (is_hirs_full_() || is_cache_full()) {
                    HashIt hirs_back_it = hash_.find(hirs_.back().key);
                    if (hirs_back_it != hash_.end()) {
                        hirs_back_it->second->status = BlockStatus::HIR_nonresident;
                        hirs_back_it->second->content = cache_.end();
                    }
                    cache_.erase(hirs_.back().content);
                    hirs_.pop_back();
                }

                cache_.push_front(get_content(hit->key));
                hit->content = cache_.begin();
                hit->status = BlockStatus::LIR;
                stack_.splice(stack_.begin(), stack_, hit);

                if (!stack_.empty()) {
                    stack_.back().status = BlockStatus::HIR_resident;
                    hash_.erase(stack_.back().key);
                    stack_.splice(hirs_.begin(), stack_, --stack_.end());
                    prune_stack_();
                }
                return false;
        }

        return false;
    }

    void update_on_hirs_hit_(BlockIt hit)
    {
        stack_.splice(stack_.begin(), hirs_, hit);
        hirs_.splice(hirs_.begin(), hirs_, hit); 
    }

    BlockIt update_on_miss_(KeyT key, std::function<T(KeyT)> get_content)
    {
        if (is_cache_full() || (is_stack_full_() && is_hirs_full_() && size_hirs_ > 0)) {
            HashIt hirs_back_it = hash_.find(hirs_.back().key);
            if (hirs_back_it != hash_.end()) {
                hirs_back_it->second->status = BlockStatus::HIR_nonresident;
                hirs_back_it->second->content = cache_.end();
            }

            cache_.erase(hirs_.back().content);
            hirs_.pop_back();
        }


        cache_.push_front(get_content(key));
        Block new_entry = {.key = key, .status = BlockStatus::HIR_resident,
                           .content = cache_.begin()};

        if (is_stack_full_())
            hirs_.push_front(new_entry);
        else
            new_entry.status = BlockStatus::LIR;

        stack_.push_front(new_entry);
        hash_[key] = stack_.begin();

        return stack_.begin();
    }

    void remove_resident_hir_(KeyT key)
    {
        for (BlockIt it = hirs_.begin(); it != hirs_.end(); it++) {
            if (it->key == key) {
                hirs_.erase(it);
                break;
            }
        }
    }

    void prune_stack_()
    {
        while (!stack_.empty() && stack_.back().status != BlockStatus::LIR) {
            hash_.erase(stack_.back().key);
            stack_.pop_back();
        }
    }

    bool is_stack_full_()
    {
        return stack_.size() >= size_lirs_;
    }

    bool is_hirs_full_()
    {
        return hirs_.size() >= size_hirs_;
    }
};

}

#endif // CACHE_LIRS_CACHE_H_
