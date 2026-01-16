#include "bstd/memory.hpp"
#include <cstring>

void bstd::mem::memcpy_and_dealloc(void* __restrict__ dest, void* __restrict__ src, size_t n) {
    memcpy(dest, src, n);
    free(src);
}
