#include <cstdint>
#include <cstdlib>

#include "page.hpp"

void* alloc_mem(void* base, size_t size);
Page alloc_page(void* base, PageSize size = PageSize::NORMAL);

void dealloc_mem(void* base);  // TODO: Implement
void dealloc_page(Page page);  // TODO: Implement
