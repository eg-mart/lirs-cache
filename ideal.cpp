#include <iostream>
#include <vector>

#include "cache.hpp"
#include "ideal_cache.hpp"
#include "cache_test.hpp"

int main()
{
    size_t size;
    std::cin >> size;

    size_t test_count;
    std::cin >> test_count;
    std::vector<int> tests(test_count);
    for (int i = 0; i < test_count; i++)
        std::cin >> tests[i];

    auto cache = cachelib::IdealCache<int, int>(size, tests.begin(), tests.end());

    bool verbose = false;
#ifdef DEBUG_OUTPUT
    verbose = true;
#endif // DEBUG_OUTPUT

    int hits = test_cache(cache, tests.begin(), tests.end(), verbose);
    std::cout << hits << "\n";

    return 0;
}
