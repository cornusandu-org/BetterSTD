#include "../include/bstd/spinlock.hpp"
#include <vector>
#include <string>

struct spinlock_data {
    spinlock_t id;
    volatile unsigned char acquired;
    std::string name;
};

std::vector<spinlock_data> spinlocks;
spinlock_t next_id = 1;

spinlock_t create_spinlock(const char* name) {
    spinlock_data data{.id = next_id, .acquired = 0, .name = std::string(name)};
    spinlocks.push_back(data);
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
    for (spinlock_data &data : spinlocks) {
        while (data.acquired == 1) {
            ;;;
        }
        data.acquired = 1;
    }
}

void release_spinlock(spinlock_t lock) {
    for (spinlock_data &data : spinlocks) {
        if (data.acquired == 1) {
            data.acquired = 0;
        }
    }
}
