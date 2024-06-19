#ifndef CACHE_HPP
#define CACHE_HPP

#include <systemc>
#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "structs.hpp"
#include <cmath>

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
};

#endif