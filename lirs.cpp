#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#include "cache.hpp"
#include "lirs_cache.hpp"
#include "cache_test.hpp"

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

    size_t test_count = 0;
    std::cin >> test_count;
    std::vector<int> tests(test_count); 
    for (size_t i = 0; i < test_count; i++) {
        std::cin >> tests[i];
    }

    auto cache = cachelib::LIRSCache<int, int>(lir_size, hir_size);

    bool verbose = false;
#ifdef DEBUG_OUTPUT
    verbose = true;
#endif // DEBUG_OUTPUT

    int hits = test_cache(cache, tests.begin(), tests.end(), verbose); 

    std::cout << hits << "\n";

    return 0;
}
