#pragma once

#include <random>

#define SEED RAND
#define PRINT_SEED 0

namespace tree::testing
{
    auto get_seed()
    {
#if SEED == RAND
        std::random_device rd;
        const auto seed = rd();
#else
        const auto seed = SEED;
#endif

#if PRINT_SEED == 1
        std::cout << "SEED: " << seed << "\n";
#endif
        return seed;
    }
}