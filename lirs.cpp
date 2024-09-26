#include <iostream>
#include <algorithm>
#include <cmath>

#include "cache.hpp"
#include "lirs_cache.hpp"

int slow_get_page(int key)
{
    return key;
}

int test_cache_verbose(cachelib::ICache<int, int, int(*)(int)>& cache, size_t test_count)
{
    int x;
    int hits = 0;
    for (size_t i = 0; i < test_count; i++) {
        std::cin >> x;

        if (cache.lookup_update(x, slow_get_page)) {
            std::cout << "HIT!" << std::endl;
            hits++;
        } else {
            std::cout << "NO HIT." << std::endl;
        }

        std::cout << "DUMP:" << std::endl;
        cache.dump();
        std::cout << std::endl;
    }

    return hits;
}

int test_cache(cachelib::ICache<int, int, int(*)(int)>& cache, size_t test_count)
{
    int x;
    int hits = 0;
    for (size_t i = 0; i < test_count; i++) {
        std::cin >> x;
        
        if (cache.lookup_update(x, slow_get_page))
            hits++;
    }

    return hits;
}

int main()
{
    size_t size, lir_size, hir_size;

#ifdef SET_HIRS_SIZE
    std::cin >> size >> hir_size;
    lir_size = std::max<size_t>(size - hir_size, 1);
#else
    std::cin >> size;
    hir_size = std::floor(std::cbrt(size));
    lir_size = std::max<size_t>(size - hir_size, 1);
#endif // SET_HIRS_SIZE

    if (hir_size < 1 || lir_size < 1) {
        std::cerr << "Error: size must be greater than 1" << std::endl;
        return 1;
    }

    size_t test_count;
    std::cin >> test_count;

    auto cache = cachelib::LIRSCache<int, int, int(*)(int)>(lir_size, hir_size);

    int hits = 0;

#ifdef DEBUG_OUTPUT
    hits = test_cache_verbose(cache, test_count); 
#else
    hits = test_cache(cache, test_count);
#endif // DEBUG_OUTPUT

    std::cout << hits << std::endl;

    return 0;
}
