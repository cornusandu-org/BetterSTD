enum class PageSize {
    NORMAL = 4096,  // 4KiB
    LARGE = 2097152,  // 2MiB
    HUGE = 1073741824  // 1GiB
};

struct Page {
    void* location;
    PageSize size;
};