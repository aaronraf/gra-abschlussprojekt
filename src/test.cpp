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
        Request request[6];
        request[0].we = 1;
        request[0].addr = 0x12;
        request[0].data = 5;

        request[1].we = 1;
        request[1].addr = 0x39;
        request[1].data = 7;

        request[2].we = 1;
        request[2].addr = 0x5;
        request[2].data = 41;

        // read from cache
        request[3].we = 0;
        request[3].addr = 0x12;

        request[4].we = 0;
        request[4].addr = 0x39;

        request[5].we = 0;
        request[5].addr = 0x5;

        Result result;
        CACHE_MODULE cache("cache", 6, 1, 32, 4, 1, 2, 6, request, nullptr);
        cache.clk(clk);

        sc_start(6, SC_SEC);

        cout << "no. of cycles: " << cache.result.cycles << endl;

        return 0;
    }

    // g++ -std=c++14 -I../../systemc/include -L../../systemc/lib -o cache_module direct_mapped_cache.cpp four_way_lru_cache.ccpplru_cache.cpp main_memory.cpp test.cpp -lsystemc
    // g++ -std=c++14 -I../../systemc/include -L../../systemc/lib -Wl,-rpath,../../systemc/lib -o cache_module direct_mapped_cache.cpp four_way_lru_cache.cpp lru_cache.cpp main_memory.cpp test.cpp -lsystemc