#include "rand.hpp"

#include <dlfcn.h>
#include <stdexcept>
#include <cstring>
#include <unordered_map>

namespace bstd::random {

    uniform_fn uniform = nullptr;
    randint_fn randint = nullptr;

    void load(const char* so_path) {
        void* handle = dlopen(so_path, RTLD_NOW);
        if (!handle) {
            throw std::runtime_error(dlerror());
        }
        
        void* sym = dlsym(handle, "uniform");
        if (!sym) {
            throw std::runtime_error(dlerror());
        }
        std::memcpy(&uniform, &sym, sizeof(uniform));

        sym = dlsym(handle, "randint");
        if (!sym) {
            throw std::runtime_error(dlerror());
        }
        std::memcpy(&randint, &sym, sizeof(randint));
    }
}
