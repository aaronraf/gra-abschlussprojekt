#ifndef DIRECTMAPPEDCACHE_HPP
#define DIRECTMAPPEDCACHE_HPP

#include "cache_address.hpp"
#include "main_memory.hpp"
#include "cache_template.hpp"
#include "cache_config.hpp"


struct CacheEntry {
    // TODO: change datatypes
    int tag;
    int data[100];
};

class DirectMappedCache : public Cache {
private:
    // TODO: change size to NoOfCacheLine
    CacheEntry cache_entry[100];

    void replace(int address, CacheEntry current_entry, int number_of_offset);

public:
    int read_from_cache(int address, CacheConfig cache_config) override;

    void write_to_cache(int address, CacheConfig cache_config, int data_to_write) override;
};

#endif