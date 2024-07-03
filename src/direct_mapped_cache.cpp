#include "../includes/direct_mapped_cache.hpp"
#include "../includes/main_memory_global.hpp"
#include <iostream>

DirectMappedCache::DirectMappedCache() {
    for (int i = 0; i < 100; ++i) {
        cache_entry[i] = new CacheEntry(); // Allocate new CacheEntry object
        cache_entry[i]->tag = 0; // Initialize tag to 0
        for (int j = 0; j < 100; ++j) {
            cache_entry[i]->data[j] = 0; // Initialize data to 0
        }
    }
}

void DirectMappedCache::replace(uint32_t address, CacheEntry* current_entry, int number_of_offset) {
    int start_address_to_fetch = (address / number_of_offset) * number_of_offset;
    int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;

    for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
        current_entry->data[offset] = main_memory->read_from_ram(ram_address);
    }
}

int DirectMappedCache::read_from_cache(uint32_t address, CacheConfig &cache_config) {
    CacheAddress cache_address(address, cache_config);
    std::cout << "index: " << cache_address.index << std::endl;
    std::cout << "tag: " << cache_address.tag << std::endl;
    std::cout << "offsett: " << cache_address.offset << std::endl;

    CacheEntry* current_entry = cache_entry[cache_address.index];
    //bool ram = false;
    std::cout << "dmc data: " << current_entry->data[cache_address.offset] << std::endl;
    
    // check if tag is in cache
    if (current_entry->tag != cache_address.tag) {
        replace(address, current_entry, cache_config.number_of_offset);
        current_entry->tag = cache_address.tag;
        //ram = true;
    }
    
    //std::cout << "ram bool: " << ram << std::endl;
    return current_entry->data[cache_address.offset];
}

void DirectMappedCache::write_to_cache(uint32_t address, CacheConfig &cache_config, int data_to_write) {
    CacheAddress cache_address(address, cache_config);
    CacheEntry* current_entry = cache_entry[cache_address.index];
    
    if (current_entry->tag != cache_address.tag) {
        replace(address, current_entry, cache_config.number_of_offset);
        current_entry->tag = cache_address.tag;
    }
    
    current_entry->data[cache_address.offset] = data_to_write;
    main_memory->write_to_ram(address, data_to_write);
}

// adit kontol