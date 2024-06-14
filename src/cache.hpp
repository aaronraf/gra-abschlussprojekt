#ifndef CACHE_HPP
#define CACHE_HPP

#include <systemc>
#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "structs.hpp"

using namespace sc_core;
using namespace std;

SC_MODULE(CACHE) {
    sc_in<Request[]> request;
    sc_out<Result> result;
    int index_counter;
    sc_in<bool> clk;

    // variables from run_simulation
    int cycles;
    int directMapped;
    unsigned cacheLines;
    unsigned cacheLineSize;
    // unsigned cacheLatency;
    // unsigned memoryLatency;
    // size_t numRequests;
    // struct Request requests[numRequests];
    // const char* tracefile;

    SC_CTOR(CACHE) {
        SC_CTHREAD(update, clk.pos());
    };

    void update() {
        cycles++;
        vector<int> address = splitAddress();

    }

    void write() {
        
    }

    void read() {

    }

    vector<int> splitAddress() {
        vector<int> array(3);
        unsigned cacheSize = cacheLines * cacheLineSize;

        array[2] = cacheLineSize / 2;                                       // offset bits
        array[1] = directMapped == 1 ? cacheLines / 2 : cacheLines / 8;     // index bits
        array[0] = cacheSize - array[1] - array[2];                         // tag bits
        return array;
    }
};

#endif