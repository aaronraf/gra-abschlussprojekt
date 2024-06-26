// 4-Way Associative Cache //
#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP
#include "cache_address.hpp"

// TODO: adjust, shouldn't be hardcoded
const int NUMBER_OF_OFFSET = 4;
int number_of_offset = 5, number_of_index = 5;

// TODO: delete this
MainMemory main_memory;

class LRUCache {
private:
    class Node {
    public:
        // TODO: sc_bv<8> data[number_of_offset];
        int data[NUMBER_OF_OFFSET];
        int map_key; // key of map which is tag
        bool is_first_time;
        Node* next;
        Node* prev;

        Node(); // constructor: initialize all data to 0 and is_first_time to true
    };

    // TODO: unordered_map<sc_bv<8>, Node*> map;
    unordered_map<int, Node*> map; // int = tag
    Node* head; // head = MRU
    Node* tail; // tail = LRU
    
    void push_to_head(Node* node); // move LRU to MRU
    void add(Node* node);
    void remove(Node* node);
        
public:
    // doubly linkedlist: head - 4 nodes - tail with O(1) replace
    // map: {key 1: node 1 ; key 2: node 2; key 3: node 3 ; key 4: node 4} with O(1) read/write

    LRUCache(); // constructor: initialize linkedlist and map
    
    ~LRUCache(); // destructor: ensure delete all nodes to avoid memory leaks

    // TODO: sc_bv<8> read(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset)
    int read_from_cache(int address);

    // TODO : void write(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset, sc_bv<8> data)
    void write_to_cache(int address, int data_to_write);

    void replace_lru(int address, int cache_address_tag);
};

#endif