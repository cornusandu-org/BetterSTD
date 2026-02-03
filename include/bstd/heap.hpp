#pragma once

#include <cstdlib>

namespace bstd {
namespace heap {
extern void init();
extern void *alloc(size_t size);
extern void dealloc(void* base);
}
}
