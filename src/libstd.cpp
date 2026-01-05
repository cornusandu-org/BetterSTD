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

void* alloc_mem(void* base, size_t size) {
    #ifdef linx

    void* mem = mmap(base, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return mem;

    #else

    void* mem = VirtualAlloc(base, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

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
