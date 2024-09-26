#include <iostream>
#include <vector>

#include "cache.hpp"
#include "ideal_cache.hpp"

int slow_get_page(int key)
{
    return key;
}

int test_cache_verbose(cachelib::ICache<int, int>& cache, std::vector<int> tests)
{
    int hits = 0;
    for (size_t i = 0; i < tests.size(); i++) {
        if (cache.lookup_update(tests[i], slow_get_page)) {
            std::cout << "HIT!" << std::endl;;
            hits++;
        } else {
            std::cout << "NO HIT." << std::endl;;
        }

        std::cout << "DUMP:" << std::endl;
        cache.dump();
        std::cout << std::endl;
    }

    return hits;
}

int test_cache(cachelib::ICache<int, int>& cache, std::vector<int> tests)
{
    int hits = 0;
    for (size_t i = 0; i < tests.size(); i++) {
        if (cache.lookup_update(tests[i], slow_get_page))
            hits++;
    }

    return hits;
}

int main()
{
    size_t size;
    std::cin >> size;

    size_t test_count;
    std::cin >> test_count;

    std::vector<int> tests;
    int tmp;
    for (int i = 0; i < test_count; i++) {
        std::cin >> tmp;
        tests.push_back(tmp);
    }

    auto cache = cachelib::IdealCache<int, int>(size, tests);

    int hits = 0;

#ifdef DEBUG_OUTPUT
    hits = test_cache_verbose(cache, tests); 
#else
    hits = test_cache(cache, tests);
#endif // DEBUG_OUTPUT

    std::cout << hits << std::endl;

    return 0;
}
