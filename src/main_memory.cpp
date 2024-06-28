#include "../includes/main_memory.hpp"
#include <iostream>
using namespace std;

// TODO: change this
const int MEMORY_SIZE = 100;

int MainMemory::read_from_ram(int address) {
    if (address < 0 || address >= MEMORY_SIZE) {
        cerr << "Error: Invalid memory address " << address << endl;
        return -1;
    }
    return memory[address];
}

void MainMemory::write_to_ram(int address, int data_to_write) {
    if (address < 0 || address >= MEMORY_SIZE) {
        cerr << "Error: Invalid memory address " << address << endl;
    }
    memory[address] = data_to_write;
}   