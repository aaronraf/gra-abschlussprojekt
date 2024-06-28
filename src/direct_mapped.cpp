#include "cache_address.hpp"
#include "main_memory.cpp"
#include "cache.cpp"

MainMemory main_memory;

struct CacheEntry {
    // TODO: change datatypes
    int tag;
    int data[NUMBER_OF_OFFSET];
};

class DirectMapped : public Cache {
private:
    // TODO: change size to NoOfCacheLine
    CacheEntry cache_entry[100];

    void replace(int address, CacheEntry current_entry) {
        int start_address_to_fetch = (address / number_of_offset) * number_of_offset;
        int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;
        
        for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
            current_entry.data[offset] = main_memory.read_from_ram(ram_address);
        }
    }

public:
    int read_from_cache(int address) {
        CacheAddress cache_address(address);
        CacheEntry current_entry = cache_entry[cache_address.index];
        
        if (current_entry.tag != cache_address.tag) {
            replace(address, current_entry);
            current_entry.tag = cache_address.tag;
        }
        
        return current_entry.data[cache_address.offset];
    }

    void write_to_cache(int address, int data_to_write) {
        CacheAddress cache_address(address);
        CacheEntry current_entry = cache_entry[cache_address.index];
        
        if (current_entry.tag != cache_address.tag) {
            replace(address, current_entry);
            current_entry.tag = cache_address.tag;
        }
        
        current_entry.data[cache_address.offset] = data_to_write;
        main_memory.write_to_ram(address, data_to_write);
    }
};

int main() {
    DirectMapped cache;

    // Example read and write operations
    int address_to_read = 0x12;     // Example address to read/write
    int data_to_write = 42;         // Example data to write

    // Write data to cache and main memory
    cache.write_to_cache(address_to_read, data_to_write);

    // Read data from cache
    int read_data = cache.read_from_cache(address_to_read);
    std::cout << "Read data from cache: " << read_data << std::endl;

    return 0;
}