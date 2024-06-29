#include "../includes/cache_module.hpp"
#include <systemc>
#include <systemc.h>
#include <iostream>
using namespace sc_core;
using namespace std;

int sc_main(int argc, char *argv[]) {
    cout << "a" << endl;
    sc_clock clk("clk", 1, SC_SEC);

    // write to cache
    Request request[3];
    request[0].we = 1;
    request[0].addr = 0x12;
    request[0].data = 5;

    request[0].we = 1;
    request[0].addr = 0x39;
    request[0].data = 7;

    request[0].we = 1;
    request[0].addr = 0x5;
    request[0].data = 41;

    // read from cache
    request[0].we = 0;
    request[0].addr = 0x12;

    request[0].we = 0;
    request[0].addr = 0x39;

    request[0].we = 0;
    request[0].addr = 0x5;

    Result result;
    CACHE_MODULE cache("cache", 3, 1, 32, 4, 1, 2, 3, request, nullptr);
    cache.clk(clk);

    sc_start(3, SC_SEC);

    cout << cache.result.cycles << endl;

    return 0;
}

// g++ -std=c++14 -I../../systemc/include -L../../systemc/lib -o cache_module direct_mapped_cache.cpp four_way_lru_cache.ccpplru_cache.cpp main_memory.cpp test.cpp -lsystemc
// g++ -std=c++14 -I../../systemc/include -L../../systemc/lib -Wl,-rpath,../../systemc/lib -o cache_module direct_mapped_cache.cpp four_way_lru_cache.cpp lru_cache.cpp main_memory.cpp test.cpp -lsystemc