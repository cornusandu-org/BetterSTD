#include "../include/bstd/spinlock.hpp"
#include <vector>
#include <string>
#include <thread>
#include <exception>
#include <atomic>
#include <string>
#include <format>

// Macros
#define SYNC __sync_synchronize()

// Typedefs
typedef unsigned char flag_t;

// Classes and structs
class spinlock_error : public std::runtime_error {
    public:
    explicit spinlock_error(const std::string& msg)
    : std::runtime_error(msg) {}
};

struct spinlock_data {
    spinlock_t id;
    std::atomic<flag_t> acquired{0};
    std::string name;
    std::thread::id owner;  // data races happen on this in this file, but that's something to fix later
};


// Forward-Declarations
static std::thread::id get_id(void);

// Global-scope variables
static constexpr std::memory_order memory_order_write_scrict  = std::memory_order::seq_cst;
static constexpr std::memory_order memory_order_all_strict    = std::memory_order::acq_rel;
static constexpr std::memory_order memory_order_none_relaxed  = std::memory_order::relaxed;
static constexpr std::memory_order memory_order_write_relaxed = std::memory_order::release;

static std::vector<spinlock_data> spinlocks;
static spinlock_t next_id = 1;
static struct spinlock_data spinlock_access_lock {.id=0, .name = "_internal_spinlock_access_lock0", .owner = get_id()};


// Helpers
static std::thread::id get_id(void) {
    return std::this_thread::get_id();
}

static void acq_global_lock_i(void) {
    if (spinlock_access_lock.owner == get_id()) throw std::logic_error("attempted to acquire already-owned spinlock");
    flag_t expected = 0;
    SYNC;
    while (!spinlock_access_lock.acquired.compare_exchange_weak(expected, 1, memory_order_write_scrict, memory_order_none_relaxed)) {
        expected = 0;
    }
    SYNC;
    spinlock_access_lock.owner = get_id();
}

static void rel_global_lock_i(void) {
    if (spinlock_access_lock.owner != get_id()) {
        throw std::logic_error("unlock by non-owner");
    }
    SYNC;
    spinlock_access_lock.owner = std::thread::id{};
    spinlock_access_lock.acquired.store(0, memory_order_write_relaxed);
    SYNC;
    return;
}


// Implementations

spinlock_t create_spinlock(const char* name) {
    spinlock_data data{.id = next_id, .acquired = 0, .name = std::string(name)};
    acq_global_lock_i();
    spinlocks.push_back(data);
    rel_global_lock_i();
    next_id++;
    return data.id;
}

spinlock_t get_spinlock(const char* name) {
    std::string n(name);

    for (spinlock_data &data : spinlocks) {
        if (data.name == n) {
            return data.id;
        }
    }

    return 0;
}

void acquire_spinlock(spinlock_t lock) {
    auto this_id = std::this_thread::get_id();

    for (auto& data : spinlocks) {
        if (data.id != lock) continue;
        if (data.owner == this_id) throw spinlock_error("attempted to acquire already-owned spinlock");

        flag_t expected = 0;
        while (!data.acquired.compare_exchange_weak(expected,
                                                    1,
                                                    memory_order_write_scrict,
                                                    memory_order_none_relaxed
              )) {
            expected = 0;
        }

        data.owner = this_id;
        return;
    }

    throw spinlock_error("invalid spinlock id");
}

void release_spinlock(spinlock_t lock) {
    auto this_id = std::this_thread::get_id();

    for (auto& data : spinlocks) {
        if (data.id != lock) continue;

        if (data.owner != this_id) {
            throw spinlock_error("unlock by non-owner");
        }

        data.owner = std::thread::id{};
        data.acquired.store(0, memory_order_write_relaxed);
        return;
    }

    throw spinlock_error("invalid spinlock id");
}

