#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#include "lirs_cache.hpp"
#include "cache_test.hpp"

int main()
{
    size_t size, lir_size, hir_size;
    std::cin >> size;
    if (!std::cin.good()) {
        std::cerr << "An I/O error has occured on size input" << std::endl;
        return 1;
    }

#ifdef SET_HIRS_SIZE
    std::cin >> hir_size;
    if (!std::cin.good()) {
        std::cerr << "An I/O error has occured on hir_size input" << std::endl;
        return 1;
    }
#else
    hir_size = std::floor(std::cbrt(size));
#endif // SET_HIRS_SIZE

    lir_size = std::max<size_t>(size - hir_size, 1);

    if (hir_size < 1 || lir_size < 1) {
        std::cerr << "Error: size must be greater than 1" << std::endl;
        return 1;
    }

    size_t test_count = 0;
    std::cin >> test_count;
    if (!std::cin.good()) {
        std::cerr << "An I/O error has occured on test_count input" << std::endl;
        return 1;
    }

    std::vector<int> tests(test_count); 
    for (size_t i = 0; i < test_count; i++) {
        std::cin >> tests[i];
        if (!std::cin.good()) {
            std::cerr << "An I/O error has occured on tests input" << std::endl;
            return 1;
        }
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
