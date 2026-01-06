namespace MemProtect {
    const constexpr _MemProtect NONE = 0;
    const constexpr _MemProtect READ = 0b1;
    const constexpr _MemProtect WRITE = 0b10;
    const constexpr _MemProtect EXECUTE = 0b100;
    const constexpr _MemProtect COPY = 0b1000;
};

namespace MemBehaviour {
    const constexpr _MemBehaviour NOCACHE = 1 << 0;
    const constexpr _MemBehaviour WRITECOMBINE = 1 << 1;
};

using _MemProtect = size_t;
using _MemBehaviour = size_t;
