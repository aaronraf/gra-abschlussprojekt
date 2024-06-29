#include <systemc>
#include <queue>
#include <iostream>
#include "../includes/structs.hpp"
#include "../includes/cache_module.hpp"

using namespace std;
using namespace sc_core;

// struct Result run_simulation(
//     int cycles,
//     int direct_mapped,           // 0 for direct mapped, other for four-way
//     unsigned cacheLines,
//     unsigned cacheLineSize,
//     unsigned cacheLatency,
//     unsigned memoryLatency,
//     size_t num_requests,
//     struct Request requests[num_requests],
//     const char* tracefile
// );

Result run_simulation(int cycles, bool direct_mapped,  unsigned cachelines, unsigned cacheline_size, 
                        unsigned cache_latency, int memory_latency, size_t num_requests, Request requests[], const char* tf_filename, 
                        const char* input_filename) {
    Result result;
    CACHE_MODULE cache ("cache", cycles, direct_mapped, cachelines, cacheline_size, cache_latency, memory_latency, num_requests, requests, tf_filename);
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