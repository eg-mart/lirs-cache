#include <iostream>
#include <vector>

#include "cache.hpp"
#include "ideal_cache.hpp"
#include "cache_test.hpp"

int main()
{
    size_t size;
    std::cin >> size;
    if (!std::cin.good()) {
        std::cerr << "An I/O error has occured on size input" << std::endl;
        return 1;
    }

    size_t test_count;
    std::cin >> test_count;
    if (!std::cin.good()) {
        std::cerr << "An I/O error has occured on test_count input" << std::endl;
        return 1;
    }

    std::vector<int> tests(test_count);
    for (int i = 0; i < test_count; i++) {
        std::cin >> tests[i];
        if (!std::cin.good()) {
            std::cerr << "An I/O error has occured on tests input" << std::endl;
            return 1;
        }
    }

    auto cache = cachelib::IdealCache<int, int>(size, tests.begin(), tests.end());

    bool verbose = false;
#ifdef DEBUG_OUTPUT
    verbose = true;
#endif // DEBUG_OUTPUT

    int hits = test_cache(cache, tests.begin(), tests.end(), verbose);
    std::cout << hits << "\n";

    return 0;
}
