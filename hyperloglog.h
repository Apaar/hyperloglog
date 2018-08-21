#include <iostream>
#include <memory>

#pragma once

const uint8_t clzLookup[] = {
        32, 31, 30, 30, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28};

class HyperLogLog {
public:
    HyperLogLog(uint32_t precision);
    void Add(uint32_t itemHash);
    uint64_t Count();
    double Error();

    // The other hyperloglog must be of same precision
    void Merge(HyperLogLog* other);
    uint8_t FetchRegister(int index) {
        return registers[index];
    }

private:
    double indicatorFunction();
    double alpha(uint32_t m);
    uint8_t clz32(uint32_t x);
    uint32_t countZeros();

    std::unique_ptr<uint8_t[]> registers;

    uint32_t numRegisters;
    uint32_t precision;
};
