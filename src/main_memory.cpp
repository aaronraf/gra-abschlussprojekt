#include <iostream>
#include <vector>
using namespace std;

// TODO: change data type

const int MEMORY_SIZE = 100;

class MainMemory {
private:
    int memory[MEMORY_SIZE];

public: 

    int read_from_ram(int address) {
        if (address < 0 || address >= MEMORY_SIZE) {
            cerr << "Error: Invalid memory address " address << endl;
            return -1;
        }
        return memory[address];
    }

    void write_to_ram(int address, int data_to_write) {
        if (address < 0 || address >= MEMORY_SIZE) {
            cerr << "Error: Invalid memory address " address << endl;
        }
        memory[address] = data_to_write;
    }   
}