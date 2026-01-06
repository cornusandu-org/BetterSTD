<div>
<p style="font-size: 2.05rem; font-weight: 500; margin-bottom: 0;">BetterSTD</p>
<p style="font-size: 1.4rem; font-weight: 400; margin-bottom: 0; color: var(--color-accent-fg)">An extension of the C++ Standard Library</p></div>

---

## Disclaimer
BetterSTD is **not** a replacement for the default standard library, nor is it meant to be. It is simply an extension meant to be used alongside the standard library.

## bstd/libstdc
BetterSTD also provides some re-implementations of libstdc that aim to be cross-platform. These functions are:
* `Pointer alloc_mem(Pointer base, size_t size)`: mmap / VirtualAlloc
* `Page alloc_page(Pointer base, PageSize size = PageSize::NORMAL)`: mmap / VirtualAlloc (for a single, entire page)
* `bool dealloc_mem(Pointer base, size_t len)`: munmap / VirtualFree
* `bool dealloc_page(const Page& page)`: munmap / VirtualFree (for a single, entire page)
* `bool protect_mem(Pointer base, size_t size, _MemProtect protect, _MemBehaviour behaviour)`: mprotect / VirtualProtect
* `bool protect_page(Page page, _MemProtect protect, _MemBehaviour behaviour)`: mprotect / VirtualProtect (for a single, entire page)

The `Pointer` classes are thin wrappers around `void*`.

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

## bstd/rand

BetterSTD's `rand` headers provide:
* `bstd::uniform`: Random value in the range of [0, 1]
* `bstd::randint`: Random value in the range of [a, b]
* `bstd::crpyto::unfirom`: Cryptographically-secure random value in the range of [0, 1]  (Not Implemented)
* `bstd::crypto::randint`: Cryptographically-secure random value in the range of [a, b]  (Not Implemented)
