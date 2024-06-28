#include "../includes/cache_address.hpp"
#include "../includes/main_memory.hpp"
#include "../includes/cache_template.hpp"
#include "../includes/direct_mapped_cache.hpp"

MainMemory main_memory;

void Direcreplace(int address, CacheEntry current_entry) {
    int start_address_to_fetch = (address / number_of_offset) * number_of_offset;
    int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;

    for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
        current_entry.data[offset] = main_memory.read_from_ram(ram_address);
    }
}

int DirectMappedCache::read_from_cache(int address) {
    CacheAddress cache_address(address);
    CacheEntry current_entry = cache_entry[cache_address.index];
    
    if (current_entry.tag != cache_address.tag) {
        replace(address, current_entry);
        current_entry.tag = cache_address.tag;
    }
    
    return current_entry.data[cache_address.offset];
}

void DirectMappedCache::write_to_cache(int address, int data_to_write) {
    CacheAddress cache_address(address);
    CacheEntry current_entry = cache_entry[cache_address.index];
    
    if (current_entry.tag != cache_address.tag) {
        replace(address, current_entry);
        current_entry.tag = cache_address.tag;
    }
    
    current_entry.data[cache_address.offset] = data_to_write;
    main_memory.write_to_ram(address, data_to_write);
}