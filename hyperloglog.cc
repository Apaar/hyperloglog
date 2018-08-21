#include "hyperloglog.h"
#include <cmath>
#include <memory>

HyperLogLog::HyperLogLog(uint32_t precision) {
    this->precision = precision;
    this->numRegisters = 1 << precision; // 2^precision
    std::cout << "Number of registers " << numRegisters << std::endl;
    this->registers = std::make_unique<uint8_t[]>(this->numRegisters);
}

void HyperLogLog::Add(uint32_t itemHash) {
    auto j = extractBits(itemHash, (32 - precision), 32);
    auto w = itemHash << precision | 1 << (precision - 1);

    auto zeroBits = clz32(w) + 1;
    if (zeroBits > registers[j]) {
        registers[j] = zeroBits;
    }
}

uint64_t HyperLogLog::Count() {
    auto est = indicatorFunction();
    if (est <= double(numRegisters) * 2.5) {
        auto count = countZeros();
        if (count != 0) {
            return uint64_t(double(numRegisters) *
                            std::log(double(numRegisters) / double(count)));
        }
    } else if (est < (twoPow32() / 30)) {
        return est;
    }
    return uint64_t(-(twoPow32()) * std::log(1 - est / (twoPow32())));
}

double HyperLogLog::Error() {
    return 1.04 / std::sqrt(numRegisters);
}

void Merge(HyperLogLog* other) {
    for (auto i = 0; i < numRegisters; i++) {
        if (other->FetchRegister(i) > registers[i]) {
            registers[i] = other->FetchRegister(i);
        }
    }
}

uint32_t HyperLogLog::countZeros() {
    uint32_t count = 0;
    for (auto i = 0; i < numRegisters; i++) {
        if (registers[i] == 0) {
            count++;
        }
    }
    return count;
}

double HyperLogLog::indicatorFunction() {
    double summation = 0;
    for (auto i = 0; i < numRegisters; i++) {
        summation += 1.0 / (double(uint64_t(1) << registers[i]));
    }
    return alpha(numRegisters) * double(numRegisters) * double(numRegisters) /
           summation;
}

double HyperLogLog::alpha(uint32_t m) {
    if (m == 16) {
        return 0.673;
    } else if (m == 32) {
        return 0.697;
    } else if (m == 64) {
        return 0.709;
    }
    return 0.7213 / (1 + 1.079 / double(m));
}

uint32_t HyperLogLog::extractBits(uint32_t num, uint32_t low, uint32_t high) {
    auto m = ((1 << (high - low)) - 1) << low;
    return (num & m) >> low;
}

uint8_t HyperLogLog::clz32(uint32_t x) {
    uint8_t n = 0;

    if (x >= (1 << 16)) {
        if (x >= (1 << 24)) {
            if (x >= (1 << 28)) {
                n = 28;
            } else {
                n = 24;
            }
        } else {
            if (x >= (1 << 20)) {
                n = 20;
            } else {
                n = 16;
            }
        }
    } else {
        if (x >= (1 << 8)) {
            if (x >= (1 << 12)) {
                n = 12;
            } else {
                n = 8;
            }
        } else {
            if (x >= (1 << 4)) {
                n = 4;
            } else {
                n = 0;
            }
        }
    }
    return clzLookup[x >> n] - n;
}