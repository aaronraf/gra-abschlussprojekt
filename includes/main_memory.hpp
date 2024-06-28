#ifndef MAINMEMORY_HPP
#define MAINMEMORY_HPP

class MainMemory {
private:
    // TODO: change size
    int memory[100];

public: 
    int read_from_ram(int address);

    void write_to_ram(int address, int data_to_write);
};

#endif