#include <systemc>
#include "lru_cache.hpp"
#include "main_memory.hpp"

SC_MODULE(CacheModule) {
    sc_in<bool> clk;
    sc_in<bool> read_en;
    sc_in<bool> write_en;
    sc_in<sc_bv<8>> address;
    sc_in<sc_bv<8>> data_in;
    sc_out<sc_bv<8>> data_out;

    LRUCache cache;
    MainMemory main_memory;

    SC_CTOR(CacheModule) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }

    void process() {
        if (read_en.read()) {
            data_out.write(cache.get(address.read()));
        } else if (write_en.read()) {
            sc_bv<8> value[4];
            for (int i = 0; i < 4; ++i) {
                value[i] = data_in.read(); // For simplicity, using the same data_in for all slots
            }
            cache.put(address.read(), value);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> clk;
    sc_signal<bool> read_en;
    sc_signal<bool> write_en;
    sc_signal<sc_bv<8>> address;
    sc_signal<sc_bv<8>> data_in;
    sc_signal<sc_bv<8>> data_out;

    CacheModule cache("CacheModule");
    cache.clk(clk);
    cache.read_en(read_en);
    cache.write_en(write_en);
    cache.address(address);
    cache.data_in(data_in);
    cache.data_out(data_out);

    sc_start(1, SC_NS);
    return 0;
}
