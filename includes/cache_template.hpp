#ifndef CACHETEMPLATE_HPP
#define CACHETEMPLATE_HPP

#include "cache_config.hpp"

class Cache {
public:
    virtual int read_from_cache(int address, CacheConfig cache_config) = 0;
    virtual void write_to_cache(int address, CacheConfig cache_config, int data_to_write) = 0;
};

#endif