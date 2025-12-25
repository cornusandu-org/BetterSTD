#pragma once
#include <cstdint>

namespace bstd {
namespace random {

    // Core RNG API
    double uniform();
    uint64_t randint(uint64_t min, uint64_t max);

    namespace crypto {
        double uniform();
        uint64_t randint(uint64_t min, uint64_t max);
    }

} // namespace random
} // namespace bstd
