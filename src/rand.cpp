#include <cstdint>
#include <cstdlib>
#include <ctime>

#define EXPORT extern "C" __attribute__((visibility("default")))

EXPORT double uniform() {
    return ((double)std::rand()) / (double)RAND_MAX;
}

EXPORT uint64_t randint(uint64_t min, uint64_t max) {
    return (uint64_t)((uniform()) * (max - min + 1) + min);
}
