#include "memory.hpp"
#include <vector>

std::vector<bstd::fixed_ptr_base*> list;

template <typename T>
bstd::fixed_ptr<T>::fixed_ptr(T *ptr) : ptr(ptr) {
    list.push_back(this);
};

template <typename T>
bstd::fixed_ptr<T>::~fixed_ptr() {
    // ...
}