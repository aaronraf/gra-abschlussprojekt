#ifndef DIRECTMAPPEDCACHE_HPP
#define DIRECTMAPPEDCACHE_HPP

#include "../includes/cache_address.hpp"
#include "../includes/main_memory.hpp"
#include "../includes/cache_template.hpp"

MainMemory main_memory;

struct CacheEntry {
    // TODO: change datatypes
    int tag;
    int data[NUMBER_OF_OFFSET];
};

class DirectMappedCache : public Cache {
private:
    // TODO: change size to NoOfCacheLine
    CacheEntry cache_entry[100];

    void replace(int address, CacheEntry current_entry);

public:
    int read_from_cache(int address) override;

    void write_to_cache(int address, int data_to_write) override;
};

#endif