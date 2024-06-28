#include <systemc>
#include <systemc.h>
#include "../includes/cache_template.hpp"
#include "../includes/four_way_lru_cache.hpp"
#include "../includes/direct_mapped_cache.hpp"
#include "../includes/main_memory.hpp"
#include "../includes/structs.hpp"
#define CACHE_ADDRESS_LENGTH 32;
using namespace std;
using namespace sc_core;

SC_MODULE(CACHE) {

    // sc_vector<sc_in<Request>> request;
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

    SC_CTOR(CACHE);
    CACHE(int cycles, int directMapped, unsigned cacheLines, unsigned cacheLineSize,
            unsigned cacheLatency, unsigned memoryLatency, size_t numRequests,
                Request request[], const char* tracefile) {
        
        this->cycles = cycles;
        this->directMapped = directMapped;
        this->cacheLines = cacheLines;
        this->cacheLineSize = cacheLineSize;
        this->cacheLatency = cacheLatency;
        this->memoryLatency = memoryLatency;
        this->numRequests = numRequests;
        this->requests = new Request[numRequests];
        for (size_t i = 0; i < numRequests; i++) {
            this->requests[i] = requests[i];
        }

        // poly cache
        if (directMapped == 0) {
            cache = new FourWayLRUCache();
        } else {
            cache = new DirectMappedCache();
        }

        // determine number of index, offset, tag
        number_of_index = (int) pow(2, ceil(log2((directMapped == 1) ? cacheLines : cacheLines / 4)));
        number_of_offset = (int) pow(2, ceil(log2(cacheLineSize)));
        number_of_tag = CACHE_ADDRESS_LENGTH - number_of_index - number_of_offset;

        SC_THREAD(update);
        sensitive << clk.pos();
    }

    ~CACHE() {
        delete[] requests;
        delete cache;
    }

    void update() {
        for (int i = 0; i < cycles; i++) {
            Request current_request = requests[i];
            // TODO : update cache and miss
            if (current_request.we) {
                cache->write_to_cache(current_request.addr, current_request.data);
            } else {
                cache->read_from_cache(current_request.addr);
            }
            result.cycles++;
            wait();
        }
    }

};

// int sc_main(int argc, char* argv[]) {
//     sc_signal<bool> clk;
//     sc_signal<bool> read_en;
//     sc_signal<bool> write_en;
//     sc_signal<sc_bv<8>> address;
//     sc_signal<sc_bv<8>> data_in;
//     sc_signal<sc_bv<8>> data_out;

//     CacheModule cache("CacheModule");
//     cache.clk(clk);
//     cache.read_en(read_en);
//     cache.write_en(write_en);
//     cache.address(address);
//     cache.data_in(data_in);
//     cache.data_out(data_out);

//     sc_start(1, SC_NS);
//     return 0;
// }