#include "../includes/direct_mapped_cache.hpp"

MainMemory main_memory;

void DirectMappedCache::replace(int address, CacheEntry current_entry, int number_of_offset) {
    int start_address_to_fetch = (address / number_of_offset) * number_of_offset;
    int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;

    for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
        current_entry.data[offset] = main_memory.read_from_ram(ram_address);
    }
}

int DirectMappedCache::read_from_cache(int address, CacheConfig cache_config) {
    CacheAddress cache_address(address, cache_config);
    CacheEntry current_entry = cache_entry[cache_address.index];
    
    if (current_entry.tag != cache_address.tag) {
        replace(address, current_entry, cache_config.number_of_offset);
        current_entry.tag = cache_address.tag;
    }
    
    return current_entry.data[cache_address.offset];
}

void DirectMappedCache::write_to_cache(int address, CacheConfig cache_config, int data_to_write) {
    CacheAddress cache_address(address, cache_config);
    CacheEntry current_entry = cache_entry[cache_address.index];
    
    if (current_entry.tag != cache_address.tag) {
        replace(address, current_entry, cache_config.number_of_offset);
        current_entry.tag = cache_address.tag;
    }
    
    current_entry.data[cache_address.offset] = data_to_write;
    main_memory.write_to_ram(address, data_to_write);
}