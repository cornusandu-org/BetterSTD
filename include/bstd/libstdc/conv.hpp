#pragma once

#include <cstdint>

namespace bstd {
    class Pointer;

    typedef char BOOL;
    typedef bstd::Pointer POINT;
    typedef std::uint32_t uint32_t;
    typedef bstd::uint32_t* PDWORD;
}

#define POINT bstd::POINT
#define BOOL bstd::BOOL
#define PDWORD bstd::PDWORD
