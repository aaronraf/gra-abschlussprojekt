#ifndef DIRECTMAPPEDCACHE_HPP
#define DIRECTMAPPEDCACHE_HPP

#include "cache_address.hpp"
#include "main_memory.hpp"
#include "cache_template.hpp"
#include "cache_config.hpp"
#include <cstdint>


struct CacheEntry {
    // TODO: change datatypes
    uint32_t tag;
    int data[100];
};

class DirectMappedCache : public Cache {
private:
    // TODO: change size to NoOfCacheLine
    CacheEntry* cache_entry[100];

    void replace(uint32_t address, CacheEntry* current_entry, int number_of_offset);

public:
    DirectMappedCache();

    int read_from_cache(uint32_t address, CacheConfig &cache_config) override;

    void write_to_cache(uint32_t address, CacheConfig &cache_config, int data_to_write) override;
};

// adit sayang

#endif