#pragma once

#include <cstdint>
#include <cstdlib>

#include "page.hpp"
#include "memprot.hpp"

void* alloc_mem(void* base, size_t size);
Page alloc_page(void* base, PageSize size = PageSize::NORMAL);

bool dealloc_mem(void* base, size_t len);  // TODO: Implement
bool dealloc_page(const Page& page);  // TODO: Implement

bool protect_mem(void* base, size_t size, _MemProtect protect, _MemBehaviour behaviour);
bool protect_page(Page page, _MemProtect protect, _MemBehaviour behaviour);
