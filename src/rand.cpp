#include "rand.hpp"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <stdexcept>

namespace {

    // One-time initialization
    struct rng_init {
        rng_init() {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
        }
    };

    rng_init init_guard;

}

namespace bstd {
namespace random {

    double uniform() {
        return static_cast<double>(std::rand())
             / static_cast<double>(RAND_MAX);
    }

    uint64_t randint(uint64_t min, uint64_t max) {
        if (min > max) {
            return min; // or swap, or assert — your policy
        }

        double u = uniform();
        uint64_t range = max - min + 1;
        return static_cast<uint64_t>(u * range) + min;
    }

    namespace crypto {

        double uniform() {
            std::runtime_error("Cryptographically secure PRNG is not yet implemented. Please use non-crypto implementations, or use an external library.");
        }

        uint64_t randint(uint64_t min, uint64_t max) {
            std::runtime_error("Cryptographically secure PRNG is not yet implemented. Please use non-crypto implementations, or use an external library.");
        }

    }

}
}
