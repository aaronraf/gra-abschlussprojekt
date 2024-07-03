#include <iostream>
#include <unordered_map>
#include <cmath>
#include <algorithm> // std::fill()
#include "../includes/four_way_lru_cache.hpp"
#include "../includes/lru_cache.hpp"
#include "../includes/cache_config.hpp"
using namespace std;

FourWayLRUCache::FourWayLRUCache(CacheConfig &cache_config) {
    // instantiate objects as many as number of index
    for (int i = 0; i < cache_config.number_of_index; i++) {
        cache_sets.push_back(new LRUCache(cache_config));
    }
}

FourWayLRUCache::~FourWayLRUCache() {
    // delete all LRUCache instances
    for (auto lru_cache : cache_sets) {
        delete lru_cache;
    }
}

int FourWayLRUCache::read_from_cache(uint32_t address, CacheConfig &cache_config) {
    CacheAddress cache_address(address, cache_config);
    uint32_t set_index = cache_address.index;
    return cache_sets[set_index]->read_from_cache(address, cache_address, cache_config);
}

void FourWayLRUCache::write_to_cache(uint32_t address, CacheConfig &cache_config, int data_to_write) {
    CacheAddress cache_address(address, cache_config);
    uint32_t set_index = cache_address.index;
    cache_sets[set_index]->write_to_cache(address, cache_address, cache_config, data_to_write);
}