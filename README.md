# BetterSTD
#### An extension of the C++ Standard Library

---

> [!IMPORTANT]  
> BetterSTD is **not** a replacement for the default standard library, nor is it meant to be. It is simply an extension meant to be used alongside the standard library.

## bstd/libstdc
BetterSTD also provides some re-implementations of libstdc that aim to be cross-platform. These functions are:
* `POINT alloc_mem(POINT base, size_t size)`: mmap / VirtualAlloc
* `Page alloc_page(POINT base, PageSize size = PageSize::NORMAL)`: mmap / VirtualAlloc (for a single, entire page)
* `BOOL dealloc_mem(POINT base, size_t len)`: munmap / VirtualFree
* `BOOL dealloc_page(const Page& page)`: munmap / VirtualFree (for a single, entire page)
* `BOOL protect_mem(POINT base, size_t size, _MemProtect protect, _MemBehaviour behaviour)`: mprotect / VirtualProtect
* `BOOL protect_page(Page page, _MemProtect protect, _MemBehaviour behaviour)`: mprotect / VirtualProtect (for a single, entire page)

The `Pointer` class is a thin wrapper around `void*`.

> [!CAUTION]
> We advise against use of `Pointer` in normal code, unless the pointer of origin is, in itself, a `void*`.

The `Page` class is a struct, containing relevant information about the page:
```cpp
struct Page {
    Pointer location;
    PageSize size;
    _MemProtect prot = MemProtect::READ | MemProtect::WRITE;
    _MemBehaviour behaviour = 0;
};
```

The page sizes are `PageSize::NORMAL` (4KiB), `PageSize::LARGE` (2MiB), `PageSize::HUGE` (1GiB).

The MemProtect values (combine with `|`) are: `MemProtect::NONE`, `MemProtect::READ`, `MemProtect::WRITE`, `MemProtect::EXECUTE`, `MemProtect::COPY`.

The MemBehaviour values (combine with `|`) are: `MemBehaviour::NOCACHE`, `MemBehaviour::WRITECOMBINE`.

> [!CAUTION]  
> `MemBehaviour::NOCACHE` and `MemBehaviour::WRITECOMBINE` can only be fully implemented on Windows, but are "simulated" on Linux via `madvise()`.

You will also have top-level names **replacing** those of `windows.h` with cross-platform alternatives:
* `POINT` -> `bstd::POINT` -> `bstd::Pointer`
* `BOOL` -> `bstd::BOOL` -> `char`
* `PDWORD` -> `bstd::PDWORD` -> `bstd::uint32_t*`  ->  `std::uint32_t*`

> [!NOTE]
> This list may be expanded in the future, and aliases may change.

## bstd/rand

BetterSTD's `rand` headers provide:
* `bstd::uniform`: Random value in the range of [0, 1]
* `bstd::randint`: Random value in the range of [a, b]
* `bstd::crypto::uniform`: Cryptographically-secure random value in the range of [0, 1]  (Not Implemented)
* `bstd::crypto::randint`: Cryptographically-secure random value in the range of [a, b]  (Not Implemented)
