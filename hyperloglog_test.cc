#include "hyperloglog.h"
#include "murmurhash3.h"
#include <iostream>

int main() {
    HyperLogLog hll(16);
    std::cout << hll.Error() << "\n";
    uint64_t n = 10000;
    for (auto i = 0; i < n; i++) {
        std::string s = std::to_string(i);
        char const* pchar = s.c_str();

        uint32_t itemHash = 0;
        MurmurHash3_x86_32(pchar, sizeof(std::to_string(i)), 0x1234, &itemHash);
        // std::cout<<itemHash<<std::endl;
        hll.Add(itemHash);
    }
    auto count = hll.Count();
    std::cout << count << " " << hll.Error() * double(count) << std::endl;
    return 0;
}