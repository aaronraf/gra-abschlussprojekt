#include <systemc>
#include <queue>

struct Request {
    uint32_t addr;
    uint32_t data;
    int we;         // 0 for read, 1 for write
}

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

struct Result run_simulation(
    int cycles,
    int directMapped,           // 0 for direct mapped, other for four-way
    unsigned cacheLines,
    unsigned cacheLineSize,
    unsigned cacheLatency,
    unsigned memoryLatency,
    size_t numRequests,
    struct Request requests[numRequests],
    const char* tracefile
);

int sc_main(int argc, char* argv[]) {
    return 0;
}