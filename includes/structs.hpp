#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <stdint.h>
#include <stdio.h>
#include <cstdint>

typedef struct Request {
    uint32_t addr ;
    uint32_t data ;
    int we ;
} Request;

typedef struct Result {
    size_t cycles ;
    size_t misses ;
    size_t hits ;
    size_t primitiveGateCount ;
} Result;

#endif