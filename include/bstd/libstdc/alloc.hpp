#pragma once

#include <cstdint>
#include <cstdlib>

class Pointer;

#include "page.hpp"
#include "memprot.hpp"
#include "os.hpp"
#include "conv.hpp"

#define POINT bstd::POINT
#define BOOL bstd::BOOL

POINT alloc_mem(POINT base, size_t size);
Page alloc_page(POINT base, PageSize size = PageSize::NORMAL);

BOOL dealloc_mem(POINT base, size_t len);
BOOL dealloc_page(const Page& page);

BOOL protect_mem(POINT base, size_t size, _MemProtect protect, _MemBehaviour behaviour);
BOOL protect_page(Page page, _MemProtect protect, _MemBehaviour behaviour);
