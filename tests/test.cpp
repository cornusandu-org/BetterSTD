// Build with `make devtest`

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cstdint>

#include "bstd/rand.hpp"
#include "bstd/re_printf.hpp"

static void require(bool condition, const char* expr) {
    if (!condition) {
        std::cerr << "REQUIRE failed: " << expr << std::endl;
        std::abort();
    }
}

static void test_require_no_error() {
    require(bstd::random::uniform() != -1, "uniform() != -1");
    require(bstd::random::randint(10, 1000) != -1, "randint(10,1000) != -1");
}

static void test_not_the_same() {
    require(
        bstd::random::randint(1, 1000) != bstd::random::randint(1, 1000),
        "randint(1,1000) produces different values"
    );

    require(
        bstd::random::randint(1, 500) != bstd::random::randint(1, 300),
        "randint(1,500) != randint(1,300)"
    );

    require(
        bstd::random::uniform() != bstd::random::uniform(),
        "uniform() produces different values"
    );
}

static void test_performance() {
    auto start = std::chrono::steady_clock::now();

    volatile uint64_t sink = 0;
    for (int i = 0; i < 500000000; ++i) {
        sink = bstd::random::randint(1, 1000);
    }

    auto end = std::chrono::steady_clock::now();
    require(end - start < std::chrono::seconds(3), "execution under 3 seconds");
}

int main() {
    struct {
        const char* name;
        void (*fn)();
    } tests[] = {
        { "Require No Error", test_require_no_error },
        { "Not The Same",     test_not_the_same     },
        { "Less than 3 seconds", test_performance  },
    };

    for (const auto& t : tests) {
        std::cout << "[ RUN ] " << t.name << std::endl;
        t.fn();
        std::cout << "[ OK  ] " << t.name << std::endl;
    }

    bstd::printf("Abcd");

    return 0;
}
