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

#define MATRIX_SIZE 4

// Matrix A and B for multiplication
int matrixA[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

int matrixB[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

// Resultant matrix C
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

// Function to initialize matrices in main memory (simulate)
void init_main_memory(MainMemory* main_memory) {
    // Store matrix A in main memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int addr = i * MATRIX_SIZE + j;
            main_memory->write_to_ram(addr, matrixA[i][j]);
        }
    }

    // Store matrix B in main memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int addr = MATRIX_SIZE * MATRIX_SIZE + i * MATRIX_SIZE + j;
            main_memory->write_to_ram(addr, matrixB[i][j]);
        }
    }
}

// Function to perform matrix multiplication using cache
void matrix_multiply_with_cache(CACHE_MODULE* cache_module, MainMemory* main_memory) {
    // Read matrix A and B from main memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int addrA = i * MATRIX_SIZE + j;
            int addrB = MATRIX_SIZE * MATRIX_SIZE + j * MATRIX_SIZE + i; // Transpose matrix B for cache-friendly access

            // Read matrix elements into cache
            Request readA_request;
            readA_request.we = 0;
            readA_request.addr = addrA;
            cache_module->requests[i * MATRIX_SIZE + j] = readA_request;

            Request readB_request;
            readB_request.we = 0;
            readB_request.addr = addrB;
            cache_module->requests[MATRIX_SIZE * MATRIX_SIZE + j * MATRIX_SIZE + i] = readB_request;
        }
    }

    // Perform matrix multiplication using cached values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                // // Read elements from cache
                // int addrA = i * MATRIX_SIZE + k;
                // int addrB = MATRIX_SIZE * MATRIX_SIZE + k * MATRIX_SIZE + j;

                // Perform multiplication
                int valueA = cache_module->data.read();
                int valueB = cache_module->data.read();
                resultMatrix[i][j] += valueA * valueB;
            }
        }
    }
}

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 1, SC_NS);
    sc_signal<uint32_t> addr;
    sc_signal<uint32_t> data;
    sc_signal<int> we;
    sc_signal<size_t> cycles;
    sc_signal<size_t> hits;
    sc_signal<size_t> misses;

    // Instantiate main memory
    MainMemory* main_memory = new MainMemory();

    // Initialize main memory with matrices A and B
    init_main_memory(main_memory);

    // Simulate cache module
    Request requests[2 * MATRIX_SIZE * MATRIX_SIZE]; // Adjust size according to your needs
    CACHE_MODULE cache_module("cache_module", 1000, 1, 32, 4, 1, 2, 2 * MATRIX_SIZE * MATRIX_SIZE, requests, nullptr);
    cache_module.clk(clk);

    // Tracefile initialization
    sc_trace_file* tracefile = sc_create_vcd_trace_file(argv[1]);
    if (tracefile == NULL) {
        fprintf(stderr, "Tracefile not opened.");
        exit(EXIT_FAILURE);
    }
    sc_trace(tracefile, clk, "Clock");
    sc_trace(tracefile, addr, "Address");
    sc_trace(tracefile, data, "Data");
    sc_trace(tracefile, we, "WE");
    sc_trace(tracefile, cycles, "Cycles");
    sc_trace(tracefile, misses, "Misses");
    sc_trace(tracefile, hits, "Hits");
    sc_trace(tracefile, misses, "Misses");

    // Start simulation
    sc_start();

    // Perform matrix multiplication using cache
    matrix_multiply_with_cache(&cache_module, main_memory);

    // Print result matrix C
    cout << "Result Matrix C = A * B:\n";
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << resultMatrix[i][j] << " ";
        }
        cout << "\n";
    }

    // Close tracefile
    sc_close_vcd_trace_file(tracefile);
    return 0;
}

Result run_simulation(int cycles, bool direct_mapped,  unsigned cachelines, unsigned cacheline_size, 
                        unsigned cache_latency, int memory_latency, size_t num_requests, Request requests[], const char* tf_filename, 
                        const char* input_filename) {
    Result result;
    CACHE_MODULE cache ("cache", cycles, direct_mapped, cachelines, cacheline_size, cache_latency, memory_latency, num_requests, requests, tf_filename);
    cache.update();
    result.cycles = cache.cycles;

    return result;
}