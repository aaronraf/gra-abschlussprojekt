#ifndef CACHEADDRESS_HPP
#define CACHEADDRESS_HPP

#include "cache_config.hpp"
#include <cstdint>

struct CacheAddress {
    // TODO: change data type
    uint32_t index; 
    uint32_t tag;
    uint32_t offset;

    // TODO: change data type
    CacheAddress(uint32_t address, CacheConfig cache_config) {
        offset = address & cache_config.number_of_offset;
        index = (address >> cache_config.number_of_offset) & cache_config.number_of_index;
        tag = index >> cache_config.number_of_index; 
    }
};

#endif