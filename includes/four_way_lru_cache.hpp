#ifndef FOURWAYLRUCACHE_HPP
#define FOURWAYLRUCACHE_HPP

#include "cache_address.hpp"
#include "cache_template.hpp"
#include "main_memory.hpp"
#include "cache_config.hpp"
#include "lru_cache.hpp"
#include <unordered_map>
#include <vector>
#include <cstdint>
using namespace std;

class FourWayLRUCache : public Cache {
private:
    vector<LRUCache*> cache_sets;

public:
    FourWayLRUCache(CacheConfig &cache_config);
    ~FourWayLRUCache();

    int read_from_cache(uint32_t address, CacheConfig &cache_config) override;
    void write_to_cache(uint32_t address, CacheConfig &cache_config, int data_to_write) override;
};

#endif