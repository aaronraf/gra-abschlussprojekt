#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <cstdint>
#include <cstddef>

struct Request {
    uint32_t addr;
    uint32_t data;
    int we;         // 0 for read, 1 for write
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

#endif