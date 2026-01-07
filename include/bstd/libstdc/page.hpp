#pragma once

#include "memprot.hpp"
#include "os.hpp"
#include "conv.hpp"

#define PDWORD bstd::PDWORD

namespace bstd {
    class Pointer {
        public:
                Pointer();
                explicit Pointer(void* p);
            explicit operator volatile void*() const noexcept;
            explicit operator void*() const noexcept;
            bool operator ==(const Pointer& other) const noexcept;
            bool isNull();
            #ifndef linx
            explicit operator PDWORD() const noexcept;
            #endif
        private:
            void* p;
    };
}

enum class PageSize {
    NORMAL = 4096,  // 4KiB
    LARGE = 2097152,  // 2MiB
    HUGE = 1073741824  // 1GiB
};

struct Page {
    bstd::Pointer location;
    PageSize size;
    _MemProtect prot = MemProtect::READ | MemProtect::WRITE;
    _MemBehaviour behaviour = 0;
};
