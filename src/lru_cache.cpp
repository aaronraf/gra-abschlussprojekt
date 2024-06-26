// 4-Way Associative Cache //
/*
    Assumption for this simulation
    1. there is only a single cache and ram
    2. user always access data from cache first, never directly to ram
*/ 

#include <iostream>
// TODO: #include <systemc>
#include <unordered_map>
#include <tuple>
#include <cmath>
using namespace std;

// TODO: adjust, shouldn't be hardcoded
const int NUMBER_OF_OFFSET = 4;
int number_of_offset = 5, number_of_index = 5;

struct CacheAddress {
    // TODO: change data type
    int index; 
    int tag;
    int offset;

    // TODO: change data type;
    CacheAddress(int address) {
        offset = address & number_of_offset;
        index = (address >> number_of_offset) & number_of_index;
        tag = index >> number_of_index; 
    }
}

class LRUCache {
private:
    class Node {
    public:
        // TODO: sc_bv<8> data[number_of_offset];
        int data[NUMBER_OF_OFFSET];
        int key;
        bool is_first_time;
        Node* next;
        Node* prev;

        Node(); // constructor: initialize all data to 0 and is_first_time to true
    };

    // TODO: unordered_map<sc_bv<8>, Node*> map;
    unordered_map<int, Node*> map; // int = tag_bits
    Node* head; // head = MRU
    Node* tail; // tail = LRU
    
    void pushToHead(Node* node); // move LRU to MRU
    void add(Node* node);
    void remove(Node* node);
        
public:
    // doubly linkedlist: head - 4 nodes - tail with O(1) replace
    // map: {key 1: node 1 ; key 2: node 2; key 3: node 3 ; key 4: node 4} with O(1) read/write

    LRUCache(); // constructor: initialize linkedlist and map
    
    ~LRUCache(); // destructor: ensure delete all nodes to avoid memory leaks

    // TODO: sc_bv<8> read(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset)
    int read_from_cache(int address, int offset);

    // TODO : void write(sc_bv<number_of_tag> tag, sc_bv<number_of_offset> offset, sc_bv<8> data)
    void write_to_cache(int address, int data_to_write);

    void replace_lru();
};


LRUCache::Node::Node() : next(nullptr), prev(nullptr) {
    for (int i = 0; i < NUMBER_OF_OFFSET; ++i) {
        data[i] = 0; 
    }
    is_first_time = true;
}

void LRUCache::pushToHead(Node* node) {
    remove(node);
    add(node);
}

void LRUCache::add(Node* node) {
    node->next = head->next;
    node->next->prev = node;
    node->prev = head;
    head->next = node;
}

void LRUCache::remove(Node* node) {
    Node* prev = node->prev;
    Node* next = node->next;
    prev->next = next;
    next->prev = prev;
}

LRUCache::LRUCache() {
    // add head and tail
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;

    // add 4 nodes and set each of these to become the value of map (key doesn't matter here since the node is_first_time is true)
    for (int i = 0; i < NUMBER_OF_OFFSET; i++) {
        Node* node = new Node(); 
        add(node);
        map[i] = node; //equivalent to map.insert(make_pair(i, node));
    }
}

LRUCache::~LRUCache() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

int LRUCache::read_from_cache(int address) {
    CacheAddress cache_address(address);
    // if not found or found but first time: fetch data from ram put it to lru
    if (map.find(cache_address.tag) == map.end() || (map[cache_address.tag] != nullptr && map[cache_address.tag]->is_first_time)) { // map.find() returns map.end() if not found
        lru_replace();
    }
    // read from lru
    Node* node = map[cache_address.tag]; 
    pushToHead(node);                        // replace O(1)
    return node->data[cache_address.offset]; // read O(1)
}

void LRUCache::write_to_cache(int index, int tag, int offset, int data_to_write) {
    // found: write, move LRU to MRU
    if (map.find(tag) != map.end()) {
        replace_lru();
    }
    Node* node = map[tag];
    node->data[offset] = data_to_write; // write O(1)
    pushToHead(node);                   // replace O(1)
}


    void LRUCache::replace_lru() {
        Node* lru_node = tail->prev;
        
        // get appropriate 4 (number_of_offset) byte data from ram
        // 1 -> 0 1 2 3    1 / 4 = 0   0 * 4 = 0     fetch 0 - 3
        // 2 -> 0 1 2 3    2 / 4 = 0   0 * 4 = 0     fetch 0 - 3
        // 5 -> 4 5 6 7    5 / 4 = 1   1 * 4 = 4     fetch 4 - 7
        // 9 -> 8 9 10 11  9 / 4 = 2   2 * 4 = 8     fetch 8 - 11
        int start_address_to_fetch = (address / number_of_offset) * number_of_offset ;
        int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;

        for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
            lru_node->data[offset] = read_from_ram[ram_address];
        }

        // update tag bits in map
        for (auto iter = map.begin(); iter != map.end(); ++iter) {  // O(1)
            if (iter->second == lru_node) {
                map.erase(iter->first);
                map[cache_address.tag] = lru_node;
                break;
            }
        }
    }
}
