#pragma once
#include <cstdint>
#include <cstdlib>

namespace bstd {
namespace random {

    // Core RNG API
    [[deprecated]] double uniform();
    [[deprecated]] uint64_t randint(uint64_t min, uint64_t max);

    class Generator {
        public:
            Generator();
            double uniform() noexcept;
            double *generate_batch(size_t count) noexcept;
            size_t randint(size_t low, size_t high) noexcept;
            ~Generator() = default;
        private:
            size_t seed;
    };

} // namespace random
} // namespace bstd
