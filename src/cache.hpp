#ifndef CACHE_HPP
#define CACHE_HPP

#include <systemc>
#include "structs.hpp"
#include <cmath>
#include <unordered_map>
#include "main_memory.hpp"

using namespace sc_core;
using namespace std;

struct TagLRUPair {
    int tag;
    int usage_freq;
};

SC_MODULE(CACHE) {
    Request request[100];
    sc_out<Result> result;
    int index_counter;
    sc_in<bool> clk;

    // variables from run_simulation
    int cycles;
    int directMapped;
    unsigned cacheLines;
    unsigned cacheLineSize;
    unsigned cacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    struct Request requests[numRequests];
    const char* tracefile;

    int indexAddressReq = 0;
    int number_of_index_bits;
    int number_of_offset_bits;
    int number_of_tag_bits;

    int parsedCacheLine[100][100][100];
    TagLRUPair tagLRUPair[cacheLines];

    MAIN_MEMORY main_memory;

    SC_CTOR(CACHE) : main_memory("main_memory") {
        // parse address
        number_of_index_bits = (int) pow(2, ceil(log2((directMapped == 1) ? cacheLines : cacheLines / 4)));
        number_of_offset_bits = (int) pow(2, ceil(log2(cacheLineSize)));
        number_of_tag_bits = 4;


        SC_CTHREAD(update, clk.pos());
    };

    void update() {
        cycles++;
        Request currentReq = request[indexAddressReq++];
        if (currentReq.we) {
            write(currentReq);
        } else {
            read(currentReq);
        }
    }

    void write(Request request) {
        int address = request.addr;
        int data = request.data;
        int offset = address & number_of_offset_bits;
        int index = (address >> number_of_offset_bits) & number_of_index_bits;
        int tag = index >> number_of_index_bits;
        TagLRUPair[index * 4 ]
        for (int i = 0; i < 4; i++) {
            if (tag == parsedCacheLine[index][i]) {
                result.hits++;
                parsedCacheLine[index][i][offset] = data;
            }
            tagLRUPair.usage_frequency[i * index + tag]++;
        }

        int minLRU = tagLRUPair.usage_frequency[index * 4];
        int minLRU_tag;
        for (int i = index * 4 + 1; i < index * 4 + 4; i++) {
            if (minLRU > tagLRUPair.usage_frequency[i]) {
                minLRU = tagLRUPair.usage_frequency[i];
                minLRU_tag = parsedCacheLine[index][i % 4];
            }
        }
        result.misses++;
        replace(minLRU, request, index);
    }

    byte read(Request request) {
        int address = request.addr;
        int data = request.data;
        int offset = address & number_of_offset_bits;
        int index = (address >> number_of_offset_bits) & number_of_index_bits;
        int tag = index >> number_of_index_bits;
        int dataToRead;
        bool dataFound = false;
        for (int i = 0; i < 4; i++) {
            if (tag == parsedCacheLine[index][i]) {
                result.hits++;
                dataToRead = parsedCacheLine[index][i][offset];
                dataFound = true;
            }
            tagLRUPair.usage_frequency[i * index + tag]++;
        }
        if (!dataFound) {
            return dataToRead;
        }
        result.misses++;
        replace();
        return parsedCacheLine[index][minLRU][offset];
    }
};

    int replace(int minLRU, Request request, int index) {
        if (request.we) {
            int dataAsAWhole = 0;
            int address_minLRU = 
            if (dataAsAWhole != main_memory[request.addr]) {
                main_memory[request.addr] = dataAsAWhole;
            }
        }
        else {
            parsedCacheLine[index][minLRU] = main_memory[request.addr];
        }
    }
}

#endif
