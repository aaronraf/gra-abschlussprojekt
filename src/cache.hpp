#ifndef CACHE_HPP
#define CACHE_HPP

#include <systemc>
#include <queue>
#include "structs.hpp"
#include <cmath>

using namespace sc_core;

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






        SC_CTHREAD(update, clk.pos());
    };

    void update() {
        cycles++;

    }

    void write() {

    }

    void read() {

    }

    // index 0: tag bits
    // index 1: index bits
    // index 2: offset bits
    int[] splitAddress() {
        return 
    }
}

#endif 