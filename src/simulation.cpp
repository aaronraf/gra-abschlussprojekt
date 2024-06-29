#include <systemc>
#include <queue>
#include <iostream>
#include "../includes/structs.hpp"
#include "cache_module.cpp"

using namespace std;
using namespace sc_core;

// struct Result run_simulation(
//     int cycles,
//     int directMapped,           // 0 for direct mapped, other for four-way
//     unsigned cacheLines,
//     unsigned cacheLineSize,
//     unsigned cacheLatency,
//     unsigned memoryLatency,
//     size_t numRequests,
//     struct Request requests[numRequests],
//     const char* tracefile
// );

Result run_simulation(int cycles, bool directMapped,  unsigned cachelines, unsigned cacheline_size, 
                        unsigned cache_latency, int memory_latency, size_t numRequests, Request requests[], const char* tf_filename, 
                        const char* input_filename) {
    Result result;
    CACHE cache ("cache", cycles, directMapped, cachelines, cacheline_size, cache_latency, memory_latency, numRequests, requests, tf_filename);
    cache.update();
    result.cycles = cache.cycles;

    return result;
}

int sc_main(int argc, char* argv[]) {
    sc_trace_file* tracefile = sc_create_vcd_trace_file(argv[1]);
    if (tracefile == NULL) {
        fprintf(stderr, "Tracefile not opened.");
        exit(EXIT_FAILURE);
    }

    sc_close_vcd_trace_file(tracefile);
    return 0;
}