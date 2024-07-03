#ifndef MAINMEMORY_HPP
#define MAINMEMORY_HPP

#include <cstdint>

class MainMemory {
private:
    // TODO: change size
    int memory[100];

public: 
    int read_from_ram(uint32_t address);

    void write_to_ram(uint32_t address, int data_to_write);
};

#endif