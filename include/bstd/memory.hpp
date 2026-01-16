#pragma once

#include <cstdint>
#include <cstdlib>

namespace bstd {
    namespace mem {
        void memcpy_and_dealloc(void* __restrict__ dest, void* __restrict__ source, size_t n);
    }
}
