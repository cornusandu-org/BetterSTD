#define HW_CACHE_SIZE 64
#define HEAP_CACHE_SIZE 256

#include <thread>

#include "../include/bstd/libstdc/libstdc.hpp"
#include "../include/bstd/libstdc/page.hpp"


constexpr size_t HEAP_ALIGNMENT = 16;

struct alignas(HW_CACHE_SIZE) CacheLine {
    void* base;
    char invalid;
};

struct alignas(32) HeapChunk {
    void* base;
    size_t size;
    char available;
    HeapChunk* next;
};

HeapChunk alignas(64) start;

char INIT_CALLED = 0;



#include "asserts/heap.hpp"



namespace bstd {
namespace heap {


void maintainer_thread() {
    return;  // to implement later, with intelligent prefetching
}


void init() {
    if (INIT_CALLED) return;
    INIT_CALLED = 1;
    bstd::Pointer p = bstd::Pointer(nullptr);
    Page page = alloc_page(p, PageSize::LARGE);
    if (page.location.isNull()) exit(1);

    start = {.base = (void*)page.location, .size = (size_t)PageSize::LARGE, .available=1, .next=nullptr};

    std::thread t(maintainer_thread);
    t.detach();

    return;
}

void merge_together(HeapChunk *start) {
    HeapChunk *current = start;
    HeapChunk *last = nullptr;

    while (current->next) {
        if (current->size == (size_t)PageSize::LARGE && current->available) {
            if (last) {
                Pointer loc = Pointer(current->base);
                Page page = Page{.location=loc, .size=PageSize::LARGE, .prot=MemProtect::READ | MemProtect::WRITE, .behaviour=0};
                dealloc_page(page);
                last->next=current->next;
                delete current;
            }
        }
        if (last) {
            if (last->available && current->available && (char*)last->base + last->size == (char*)current->base) {
                last->size += current->size;
                last->next = current->next;
                delete current;
                current = last->next;
                continue;
            }
        }
        last = current;
        current = current->next;
    }
}

void *alloc(size_t size) {
    HeapChunk *current = &start;
    size = (size + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1);


    while (1) {
        if (current->available && current->size >= size) {
            if (current->size == size) {
                current->available = 0;
                return current->base;
            };
            
            HeapChunk* next_chunk = current->next;
            HeapChunk* new_chunk = new HeapChunk{.base = (char*)current->base + size, .size = current->size - size, .available = 1};
            current->size = size;
            current->next = new_chunk;
            current->available=0;
            new_chunk->next = next_chunk;
            return current->base;
        }
        if (current->next == nullptr) {
            bstd::Pointer p = bstd::Pointer(nullptr);
            Page page = alloc_page(p, PageSize::LARGE);
            if (page.location.isNull()) exit(1);

            current->next = new HeapChunk{.base = (void*)page.location, .size = (size_t)PageSize::LARGE, .available=1, .next=nullptr};

            merge_together(&start);

            current = &start;
        }
    }
}

void dealloc(void* base) {
    HeapChunk *current = &start;

    while (1) {
        if (current->base == base && !current->available) {
            current->available = 1;
            break;
        }
        current = current->next;
        if (!current) break;
    }
    merge_together(&start);
}

}}