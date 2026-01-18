#pragma once

#include <cstdlib>

namespace bstd {
namespace heap {
void init();
void *alloc(size_t size);
void dealloc(void* base);
}
}