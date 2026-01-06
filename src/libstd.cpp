#if defined(LINUX) || defined(__unix__) || defined(__unix) || defined(POSIX) || defined(__LINUX__) || defined(__LINUX) || defined(__POSIX) || defined(__linux__) || defined(unix)
#warning Building for Linux
#define linx 1
#elif defined(_WIN64) || defined(__CYGWIN__)
#warning Building for Windows
#if defined(_WIN32)
#warning | Windows x32 -- may not be fully supported  !!
#endif
#else
#warning Unsupported operating system. Please note that this library may not work on your OS. (falling back to linux ABI)
#define linx 1
#endif

#include "bstd/libstdc/libstdc.hpp"

#ifndef linx
#include <windows.h>

BOOL EnableLargePages(void)
{
    HANDLE token;
    TOKEN_PRIVILEGES tp;

    if (!OpenProcessToken(GetCurrentProcess(),
                           TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                           &token))
        return FALSE;

    LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &tp.Privileges[0].Luid);

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(token, FALSE, &tp, 0, NULL, NULL);
    CloseHandle(token);

    return GetLastError() == ERROR_SUCCESS;
}

#else
#include <sys/mman.h>
#endif

//constexpr MemProtect operator|(MemProtect a, MemProtect b) noexcept
//{
//    return static_cast<MemProtect>(
//        static_cast<size_t>(a) | static_cast<size_t>(b)
//    );
//}
//
//constexpr MemProtect operator&(MemProtect a, MemProtect b) noexcept
//{
//    return static_cast<MemProtect>(
//        static_cast<size_t>(a) & static_cast<size_t>(b)
//    );
//}
//
//constexpr MemProtect operator~(MemProtect a) noexcept
//{
//    return static_cast<MemProtect>(
//        ~static_cast<size_t>(a)
//    );
//}

void* alloc_mem(void* base, size_t size) {
    #ifdef linx

    void* mem = mmap(base, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return mem;

    #else

    void* mem = VirtualAlloc(base, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    return mem;

    #endif
}

Page alloc_page(void* base, PageSize size) {
    #ifdef linx

    void* mem = mmap(base, (size_t)size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | (((size_t)size > 4096) ? (MAP_HUGETLB | ((size_t)size > 2097152 ? MAP_HUGE_1GB : MAP_HUGE_2MB)) : 0), -1, 0);

    Page page{
        .location = mem,
        .size = size
    };

    return page;

    #else

    EnableLargePages();

    void* mem = VirtualAlloc(base, (size_t)size, MEM_RESERVE | MEM_COMMIT | (((size_t)size > 4096) ? MEM_LARGE_PAGES : 0), PAGE_EXECUTE_READWRITE);

    Page page{
        .location = mem,
        .size = size
    };

    return page;

    #endif
}

bool dealloc_mem(void* base, size_t len) {
    #ifdef linx

    return munmap(base, len) == 0;

    #else

    return (bool)VirtualFree(base, len, MEM_RELEASE | MEM_DECOMMIT);

    #endif
}

bool dealloc_page(const Page& page) {
    return dealloc_mem(page.location, (size_t)page.size);
}

bool protect_mem(void* base, size_t size, _MemProtect protect, _MemBehaviour behaviour, size_t *storePROT = 0, size_t *storeBEHAV = 0) {
    #ifdef linx

    return false;  // TODO: Implement this

    #else

    size_t protection = 0;
    size_t behav = 0;

    switch (protect) {
        case MemProtect::READ:
            protection = PAGE_READONLY;
            break;
        case MemProtect::READ | MemProtect::WRITE:
            protection = PAGE_READWRITE;
            break;
        case MemProtect::READ | MemProtect::EXECUTE:
            protection = PAGE_EXECUTE_READ;
            break;
        case MemProtect::READ | MemProtect::WRITE | MemProtect::EXECUTE:
            protection = PAGE_EXECUTE_READWRITE;
            break;
        case MemProtect::READ | MemProtect::WRITE | MemProtect::COPY:
            protection = PAGE_WRITECOPY;
            break;
        case MemProtect::READ | MemProtect::EXECUTE | MemProtect::WRITE | MemProtect::COPY:
            protection = PAGE_EXECUTE_WRITECOPY;
            break;
        case MemProtect::NONE:
            protection = PAGE_NOACCESS;
            break;
        default:
            return false;
    }

    if (behaviour & MemBehaviour::NOCACHE)        behav |= PAGE_NOCACHE;
    if (behaviour & MemBehaviour::WRITECOMBINE)   behav |= PAGE_WRITECOMBINE;

    if (storePROT) *storePROT = protection;
    if (storeBEHAV) *storeBEHAV = behav;

    DWORD a = 0;

    return (bool)VirtualProtect(base, size, protection | behav, (PDWORD)&a);

    #endif
}

bool protect_page(Page &page, _MemProtect protect, _MemBehaviour behaviour) {
    size_t prot = 0;
    size_t behav = 0;
    bool s = protect_mem(page.location, (size_t)page.size, protect, behaviour, &prot, &behav);
    page.prot = prot;
    page.behaviour = behav;
    
    return s;
}
