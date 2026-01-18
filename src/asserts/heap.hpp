#include <stdint.h>
#include <stdlib.h>

extern const size_t HEAP_ALIGNMENT;

struct CacheLine;
struct HeapChunk;
 
extern HeapChunk start;

#ifndef __INTELLISENSE__

// NOLINTBEGIN(cppcoreguidelines-*, clang-diagnostic-*)

static_assert(HEAP_ALIGNMENT % 16 == 0);
static_assert(alignof(HeapChunk) >= 16);
static_assert(alignof(HeapChunk) % 8  == 0);
static_assert(alignof(start)     % 16 == 0);

// NOLINTEND

#endif
