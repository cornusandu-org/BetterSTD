#include "include/bstd/sort.hpp"
#include <iostream>
#include <cstdlib>

int main() {
    int a[] = {1, 5, 2, 4, 3, 6, 0};

    sort<int>(a, (uptr)sizeof(a) / sizeof(a[0]));

    for (int i = 0; i < 7; i++) {
        int num = a[i];

        std::printf("%d ", num);
    }
}
