#ifndef CACHE_HPP
#define CACHE_HPP

#include <systemc>
#include "structs.hpp"
#include <cmath>
#include <unordered_map>
#include "main_memory.hpp"

typdef struct TagLRUPair tagLRUPair;

using namespace sc_core;
using namespace std;

struct TagLRUPair {
    int tag;
    int offset;
    int usage_frequency;
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
    // size_t numRequests;
    // struct Request requests[numRequests];
    // const char* tracefile;

    SC_CTOR(CACHE) {
        // parse address
        int number_of_index_bits =  ceil(log2((directMapped == 1) ? cacheLines : cacheLines / 4));
        int number_of_offset_bits = ceil(log2(cacheLineSize));
        int number_of_tag_bits = (cacheLines * cacheLineSize) - number_of_index_bits - number_of_offset_bits;

        // 






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

        for (int i = 0; i < cacheLines; i++) {
            tagLRUPair[i] = new TagLRUPair;
            tagLRUPair[i].usage_frequency = i % 4;
        }

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
        int tag = index >> number_of_index_bits;        // tag of address yg kita mau write
        bool dataFound = false;
        int offset_LRU = 0;
        
        for (int i = 0; i < 4; i++) {
            if (tag == tagLRUPair[index * 4 + i].tag) {
                dataFound = true;
                result.hits++;
                parsedCacheLine[index][i][offset] = data;
            }
            tagLRUPair[index * 4 + i].usage_frequency++;
        }

        if (dataFound) {
            return;
        }

        TagLRUPair minLRU = tagLRUPair[index * 4];
        int tagindexintaglrupair = index*4;
        for (int i = index * 4 + 1; i < index * 4 + 4; i++) {
            if (minLRU.usage_frequency > tagLRUPair[i].usage_frequency) {
                minLRU.usage_frequency = tagLRUPair[i].usage_frequency;
                tagindexintaglrupair = i;

            }
        }
        
        result.misses++;
        replace(minLRU, request, index, tagindexintaglrupair, offset);

        parsedCacheLine[index][tagindexintaglrupair][offset] = data;
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

    // rep
    void replace(TagLRUPair minLRU, Request request, int index, int tag_index_but_in_lru_pair, int offset_of_req_address) {
        // write enable
        if (request.we) {
            // check data coherence between min lru in cache and ram
            int address_lru = 0 | (minLRU.tag << number_of_index_bits) | (index << number_of_offset_bits) | offset_LRU;
            if (main_memory[address_lru] != parsedCacheLine[index][tag_index_but_in_lru_pair % 4][minLRU.offset]) {
                main_memory[address_lru] = parsedCacheLine[index][tag_index_but_in_lru_pair % 4][minLRU.offset];
            }

            // get the new data in request from ram and put it to cache
            parsedCacheLine[index][tag_index_but_in_lru_pair % 4] = main_memory[request.addr];
        } 
        else {
            parsedCacheLine[index][minLRU] = main_memory[request.addr];
        }
    }
};

#endif
