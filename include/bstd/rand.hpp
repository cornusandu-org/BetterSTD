#pragma once
#include <cstdint>

namespace bstd {
    namespace random {

        using uniform_fn = double (*)();
        using randint_fn = uint64_t (*)(uint64_t, uint64_t);

        extern uniform_fn uniform;
        extern randint_fn randint;

        namespace crypto {
            extern uniform_fn uniform;
            extern randint_fn randint;
        }

        void load(const char* so_path);
    }
}
