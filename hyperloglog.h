#include <iostream>
#include <memory>

#pragma once

const uint8_t clzLookup[] = {
        32, 31, 30, 30, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28};

class HyperLogLog {
public:
    HyperLogLog(uint32_t precision);
    void Add(uint32_t itemHash);
    void Merge(HyperLogLog* other);
    uint64_t Count();

private:
    uint32_t extractBits(uint32_t hash, uint32_t low, uint32_t high);
    double indicatorFunction();
    double alpha(uint32_t m);
    uint8_t clz32(uint32_t x);
    uint32_t countZeros();
    uint64_t twoPow32() {
        return uint64_t(1) << uint64_t(32);
    }
    std::unique_ptr<uint8_t[]> registers;

    uint32_t numRegisters;
    uint32_t precision;
};
