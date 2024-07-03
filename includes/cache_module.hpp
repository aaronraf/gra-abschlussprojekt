#ifndef CACHEMODULE_HPP
#define CACHEMODULE_HPP

#include <systemc>
#include <iostream>
#include "../includes/cache_module_includes.hpp"
#include <cstdint>
#define CACHE_ADDRESS_LENGTH 32;
using namespace std;
using namespace sc_core;

MainMemory* main_memory = new MainMemory();

SC_MODULE(CACHE_MODULE) {

    // sc_vector<sc_in<Request>> request;
    sc_signal<int> data;
    sc_in<bool> clk;
    Result result;

    int cycles;
    int directMapped;
    unsigned cacheLines;
    unsigned cacheLineSize;
    unsigned cacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    Request* requests;
    const char* tracefile;

    int number_of_index, number_of_tag, number_of_offset;
    Cache* cache; 
    CacheConfig cache_config;

    SC_CTOR(CACHE_MODULE);
    CACHE_MODULE(sc_module_name name, int cycles, int directMapped, unsigned cacheLines, unsigned cacheLineSize,
                unsigned cacheLatency, unsigned memoryLatency, size_t numRequests,
                Request requests[], const char* tracefile) : sc_module(name) {
        
        this->cycles = cycles;
        this->directMapped = directMapped;
        this->cacheLines = cacheLines;
        this->cacheLineSize = cacheLineSize;
        this->cacheLatency = cacheLatency;
        this->memoryLatency = memoryLatency;
        this->numRequests = numRequests;
        this->requests = new Request[numRequests];
        for (size_t i = 0; i < numRequests; i++) {
            memcpy(&this->requests[i], &requests[i], sizeof(Request));
            //cout << "Copying request[" << i << "]: addr = " << requests[i].addr << " but " << this->requests[i].addr <<endl;

        }   

        // determine number of index, offset, tag
        cache_config.number_of_index = (int) pow(2, ceil(log2((directMapped == 1) ? cacheLines : cacheLines / 4)));
        cache_config.number_of_offset = (int) pow(2, ceil(log2(cacheLineSize)));
        cache_config.number_of_tag = CACHE_ADDRESS_LENGTH - number_of_index - number_of_offset;

        // poly cache
        // TODO: one main memory for all
        if (directMapped == 0) {
            cache = new FourWayLRUCache(cache_config);
        } else {
            cache = new DirectMappedCache();
        }

        SC_THREAD(update);
        sensitive << clk.pos();
    }

    ~CACHE_MODULE() {
        delete[] requests;
        delete cache;
    }

    void update() {
        for (int i = 0, cache_latency_count = cacheLatency; i < cycles; i++, cache_latency_count++){
            Request current_request = requests[i];
            //cout << "Processing request[" << i << "]: addr = " << current_request.addr << endl;            // TODO : update cache and miss, RAM, cache exception from ram, pass main_memory and result to access operation
            if (cache_latency_count == cacheLatency) {
                if (current_request.we) {
                    cache->write_to_cache(current_request.addr, cache_config, current_request.data);
                } else {
                    cout << "current addr: " << current_request.addr << endl;
                    cout << "no. ind: " << cache_config.number_of_index << endl;
                    cout << "no. tag: " << cache_config.number_of_tag << endl;
                    cout << "no. offset: " << cache_config.number_of_offset << endl;

                    data = cache->read_from_cache(current_request.addr, cache_config);
                    //cout << "current_request.addr: " << current_request.addr << endl;
                    cout << "cache_module_data: " << data << endl;
                }
                cache_latency_count = 0;
            }
            
            result.cycles++;
            wait();
        }
    }

};


// int sc_main() {
//     sc_clock clk("clk", 1, SC_SEC);

//     // write to cache
//     Request request[3];
//     request[0].we = 1;
//     request[0].addr = 0x12;
//     request[0].data = 5;

//     request[0].we = 1;
//     request[0].addr = 0x39;
//     request[0].data = 7;

//     request[0].we = 1;
//     request[0].addr = 0x5;
//     request[0].data = 41;

//     // read from cache
//     request[0].we = 0;
//     request[0].addr = 0x12;

//     request[0].we = 0;
//     request[0].addr = 0x39;

//     request[0].we = 0;
//     request[0].addr = 0x5;

//     Result result;
//     CACHE_MODULE cache("cache", 3, 1, 32, 4, 1, 2, 3, request, nullptr);
//     cache.clk(clk);

//     sc_start(3, SC_SEC);

//     cout << cache.result.cycles << endl;

//     return 0;
    
// }

// // g++ -I../../systemc/include -L../../systemc/lib -o cache_module cache_module.cpp direct_mapped_cache.cpp four_way_lru_cache.cpp lru_cache.cpp main_memory.cpp -lsystemc
//adit memek
#endif