#include "hyperloglog.h"
#include "murmurhash3.h"
#include <iostream>

int main() {
    HyperLogLog hll(16);
    uint64_t n = 10000;
    for (auto i = 0; i < n; i++) {
        char tmp[100];
        snprintf(tmp, sizeof(tmp), "item-%05d", i);

        uint32_t itemHash = 0;
        MurmurHash3_x86_32(tmp, 100, 0, &itemHash);
        hll.Add(itemHash);
        hll.Add(itemHash);
    }

    std::cout << hll.Count();
    return 0;
}