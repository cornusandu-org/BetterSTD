#pragma once

#include "libstdc/conv.hpp"
#include "libstdc/alloc.hpp"
#include <thread>
#include <vector>
#include <string>
#include <cstring>

template<typename T>
void _sort(T *start, T *end, uptr piv) {  // ALWAYS start < end; `end` is ALWAYS out of bounds
    if ((end - start) <= 1) return;

    T *a = start, *b = start;
    T* pivot = start + piv;
    T pivot_val = *pivot;
    std::swap(*pivot, *(end - 1));
    pivot = end - 1;

    while (b < end - 1) {
        //if (b == pivot){ ++b; continue; }
        if (*b < pivot_val) {
            std::swap(*a, *b);
            a++;
        }
        b++;
    }

    std::swap(*a, *pivot);
    pivot = a;

    uptr left_size  = pivot - start;
    uptr right_size = end - (pivot + 1);

    uptr left_piv  = left_size  >> 1;
    uptr right_piv = right_size >> 1;

    std::thread thread_a(_sort<T>, start, pivot, left_piv);
    _sort<T>(pivot + 1, end, right_piv);

    thread_a.join();

    return;
}

template<typename T>
void sort(T *arr, uptr size) {
    if (size <= 1) return;

    uptr piv = size >> 1;

    void* mem = (void*)alloc_mem(bstd::Pointer(nullptr), size * sizeof(T));

    memcpy(mem, arr, size * sizeof(T));

    _sort((T*)mem, (T*)mem + size, piv);

    memcpy(arr, mem, size * sizeof(T));

    dealloc_mem(bstd::Pointer(mem), size * sizeof(T));
}
