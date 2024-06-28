#ifndef CACHEADDRESS_HPP
#define CACHEADDRESS_HPP

#include "cache_config.hpp"

struct CacheAddress {
    // TODO: change data type
    int index; 
    int tag;
    int offset;

    // TODO: change data type
    CacheAddress(int address, CacheConfig cache_config) {
        offset = address & cache_config.number_of_offset;
        index = (address >> cache_config.number_of_offset) & cache_config.number_of_index;
        tag = index >> cache_config.number_of_index; 
    }
};

#endif