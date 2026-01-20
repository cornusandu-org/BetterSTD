#include "rand.hpp"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <stdexcept>
#include <cassert>

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

    [[deprecated]] double uniform() {
        return static_cast<double>(std::rand())
             / static_cast<double>(RAND_MAX);
    }

    [[deprecated]] uint64_t randint(uint64_t min, uint64_t max) {
        if (min > max) {
            return min;
        }

        double u = uniform();
        uint64_t range = max - min + 1;
        return static_cast<uint64_t>(u * range) + min;
    }

    Generator::Generator() {
        this->seed = static_cast<size_t>(std::time(nullptr));
    }

    double Generator::uniform() noexcept {
        std::size_t z = (seed += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        z ^= (z >> 31);

        return (z >> 11) * (1.0 / 9007199254740992.0);
    }

    double *Generator::generate_batch(size_t count) noexcept {
        double *array = new double[count];

        for (size_t i = 0; i < count; i++) {
            array[i] = this->uniform();
        }

        return array;
    }

    std::size_t Generator::randint(std::size_t low, std::size_t high) noexcept {
        if (high <= low) return low;
        
        const std::size_t range = high - low + 1;
        const std::size_t limit =
            std::numeric_limits<std::size_t>::max() -
            (std::numeric_limits<std::size_t>::max() % range);
        
        std::size_t x;
        do {
            std::size_t z = (seed += 0x9e3779b97f4a7c15ULL);
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
            x = z ^ (z >> 31);
        } while (x >= limit);
    
        return low + (x % range);
    }
}
}
