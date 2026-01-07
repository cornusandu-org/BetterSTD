#pragma once

#include <cstdint>

namespace bstd {
    class Pointer;

    typedef long unsigned int luint;
    typedef bstd::Pointer POINT;
    typedef std::uint32_t uint32_t;
    typedef luint* PDWORD;
    typedef unsigned char uint8_t;
    typedef uint8_t BOOL;
    typedef char* cstring;
}

#define POINT bstd::POINT
#define BOOL bstd::BOOL
#define PDWORD bstd::PDWORD

using u8 = bstd::uint8_t;
using u16 = std::uint16_t;
using u32 = bstd::uint32_t;
using u64 = std::uint64_t;
using int8 = signed char;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using s8 = int8;
using s16 = int16;
using s32 = int32;
using s64 = int64;
using i8 = int8;
using i16 = int16;
using i32 = int32;
using i64 = int64;
using byte = u8;
using sbyte = int8;
using schar = i8;
using cstring = bstd::cstring;
using cstr = cstring;
using luint = bstd::luint;
using uptr = uintptr_t;
