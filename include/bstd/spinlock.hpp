#include <cstdlib>
#include <exception>
#include <string>
#include <stdexcept>

using spinlock_t = size_t;

extern spinlock_t create_spinlock(const char* name);
extern spinlock_t get_spinlock(const char* name);
extern void acquire_spinlock(spinlock_t lock);
extern void release_spinlock(spinlock_t lock);

class spinlock_error : public std::runtime_error {
public:
    explicit spinlock_error(const std::string& msg);
};
