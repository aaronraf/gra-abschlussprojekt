#ifndef CACHEADDRESS_HPP
#define CACHEADDRESS_HPP

// TODO: adjust, shouldn't be hardcoded
const int NUMBER_OF_OFFSET = 4;
int number_of_offset = 5, number_of_index = 5;

struct CacheAddress {
    // TODO: change data type
    int index; 
    int tag;
    int offset;

    // TODO: change data type; adjust for direct mapped
    CacheAddress(int address) {
        offset = address & number_of_offset;
        index = (address >> number_of_offset) & number_of_index;
        tag = index >> number_of_index; 
    }
};

#endif