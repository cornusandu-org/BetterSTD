#pragma once

#include "memprot.hpp"

enum class PageSize {
    NORMAL = 4096,  // 4KiB
    LARGE = 2097152,  // 2MiB
    HUGE = 1073741824  // 1GiB
};

struct Page {
    void* location;
    PageSize size;
    _MemProtect prot = MemProtect::READ | MemProtect::WRITE;
    _MemBehaviour behaviour = 0;
};
