#include <systemc>
#include <unordered_map>
#include "structs.hpp"
using namespace sc_core;
using namespace std;

SC_MODULE(MAIN_MEMORY) {
    sc_in<Request[]> request;
    sc_out<Result> result;
    int index_counter = 0;
    unordered_map<uint32_t, int> frequency_map;
    sc_in<bool> clk;
}