// Build with `make devtest`

#include <iostream>
#include <unistd.h>
#include <dlfcn.h>
#include <cassert>
#include <chrono>

#include "bstd/rand.hpp"

#include <vector>
#include <functional>

struct Test {
    const char* name;
    void (*fn)();
};

inline std::vector<Test>& registry() {
    static std::vector<Test> tests;
    return tests;
}

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)


#define TEST_CASE(name)                                              \
    static void CONCAT(test_, __LINE__)();                           \
    static bool CONCAT(reg_, __LINE__) = [] {                         \
        registry().push_back({ name, CONCAT(test_, __LINE__) });     \
        return true;                                                  \
    }();                                                              \
    static void CONCAT(test_, __LINE__)()

#define REQUIRE(expr)                                       \
    do {                                                    \
        if (!(expr)) {                                     \
            std::cerr << "REQUIRE failed: " #expr          \
                      << "\n  at " << __FILE__             \
                      << ":" << __LINE__ << std::endl;     \
            std::abort();                                  \
        }                                                   \
    } while (0)



TEST_CASE("Require No Error") {
    REQUIRE(bstd::random::uniform() != -1);
    REQUIRE(bstd::random::randint(10, 1000) != -1);
};

TEST_CASE("Not The Same") {
    REQUIRE(bstd::random::randint(1, 1000) != bstd::random::randint(1, 1000));
    REQUIRE(bstd::random::randint(1, 500) != bstd::random::randint(1, 300));
    REQUIRE(bstd::random::uniform() != bstd::random::uniform());
};

TEST_CASE("Less than 5 seconds") {
    auto start = std::chrono::steady_clock::now();
    volatile uint64_t sink;
    for (int i = 0; i < 500000000; ++i) {
        sink = bstd::random::randint(1, 1000);
    }
    auto end = std::chrono::steady_clock::now();

    REQUIRE(end - start < std::chrono::seconds(3));
};

int main() {
    bstd::random::load("./dist/bstd.so");

    for (const auto& t : registry()) {
        std::cout << "[ RUN ] " << t.name << std::endl;
        t.fn();
        std::cout << "[ OK  ] " << t.name << std::endl;
    }

    return 0;
}
