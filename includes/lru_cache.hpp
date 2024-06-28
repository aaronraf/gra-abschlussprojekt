#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include "cache_address.hpp"
#include "cache_template.hpp"
#include "main_memory.hpp"
#include "cache_config.hpp"
#include "lru_cache.hpp"
#include <unordered_map>
#include <vector>
using namespace std;

class LRUCache {
private:
    class Node {
    public:
        // TODO: sc_bv<8> data[number_of_offset];
        int data[100];
        int map_key; // key of map which is tag
        bool is_first_time;
        Node* next;
        Node* prev;

        Node(int number_of_offset); // constructor: initialize all data to 0 and is_first_time to true
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

    LRUCache(CacheConfig cache_config); // constructor: initialize linkedlist and map
    
    ~LRUCache(); // destructor: ensure delete all nodes to avoid memory leaks

    // TODO: sc_bv<8> read(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset)
    int read_from_cache(int address, CacheAddress cache_address, CacheConfig cache_config);

    // TODO : void write(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset, sc_bv<8> data)
    void write_to_cache(int address, CacheAddress cache_address, CacheConfig cache_config, int data_to_write);

    void replace_lru(int address, int cache_address_tag, CacheConfig cache_config);
};

#endif