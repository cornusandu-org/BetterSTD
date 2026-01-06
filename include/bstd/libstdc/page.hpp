#pragma once

#include "memprot.hpp"
#include "os.hpp"

#ifndef linx
#include <windows.h>
#endif

class Pointer {
    public:
            Pointer();
            explicit Pointer(void* p);
        explicit operator volatile void*() const noexcept;
        explicit operator void*() const noexcept;
        #ifndef linx
        explicit operator PDWORD() const noexcept;
        #endif
    private:
        void* p;
};

enum class PageSize {
    NORMAL = 4096,  // 4KiB
    LARGE = 2097152,  // 2MiB
    HUGE = 1073741824  // 1GiB
};

struct Page {
    Pointer location;
    PageSize size;
    _MemProtect prot = MemProtect::READ | MemProtect::WRITE;
    _MemBehaviour behaviour = 0;
};
