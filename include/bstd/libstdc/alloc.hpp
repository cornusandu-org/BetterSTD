#pragma once

#include <cstdint>
#include <cstdlib>

class Pointer;

#include "page.hpp"
#include "memprot.hpp"
#include "os.hpp"

Pointer alloc_mem(Pointer base, size_t size);
Page alloc_page(Pointer base, PageSize size = PageSize::NORMAL);

bool dealloc_mem(Pointer base, size_t len);
bool dealloc_page(const Page& page);

bool protect_mem(Pointer base, size_t size, _MemProtect protect, _MemBehaviour behaviour);
bool protect_page(Page page, _MemProtect protect, _MemBehaviour behaviour);
