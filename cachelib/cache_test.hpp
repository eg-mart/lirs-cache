#ifndef CACHE_CACHE_TEST_H_
#define CACHE_CACHE_TEST_H_

#include <iostream>
#include "cache.hpp"

namespace cachelib {

template <typename Iterator>
int test_cache(ICache<int, int>& cache, Iterator begin, Iterator end,
               bool verbose=false)
{
    int hits = 0;
    for (Iterator it = begin; it != end; it++) {
        if (cache.lookup_update(*it, [](int key) { return key; })) {
            if (verbose)
                std::cout << "HIT!\n";
            hits++;
        } else {
            if (verbose)
                std::cout << "NO HIT.\n";
        }
    }

    if (verbose) {
        std::cout << "DUMP:\n";
        cache.dump();
        std::cout << std::endl;
    }

    return hits;
}

}

#endif // CACHE_CACHE_TEST_H_
