#include <iostream>
// TODO: #include <systemc>
#include <unordered_map>
#include <tuple>
#include <cmath>
#include <algorithm> // std::fill()
#include "main_memory.cpp"
#include "lru_cache.hpp"
using namespace std;

LRUCache::Node::Node() : next(nullptr), prev(nullptr) {
    fill(data, data + number_of_offset, 0); // initialize array to 0
    is_first_time = true;
}

void LRUCache::push_to_head(Node* node) {
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
        node->map_key = i; 
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
    // if not found -> replace
    if (map.find(cache_address.tag) == map.end()) { // map.find() returns map.end() if not found
        replace_lru(address, cache_address.tag);
    }
    // if accidentally found during first iteration so is_first_time is true -> also replace
    if (map[cache_address.tag]->is_first_time) {
        replace_lru(address, cache_address.tag);
    }
    // read from lru and update lru to mru
    Node* node = map[cache_address.tag]; 
    push_to_head(node);                        // replace O(1)
    return node->data[cache_address.offset]; // read O(1)
}

void LRUCache::write_to_cache(int address, int data_to_write) {
    CacheAddress cache_address(address);
    // if not found -> replace
    if (map.find(cache_address.tag) == map.end()) { // map.find() returns map.end() if not found
        replace_lru(address, cache_address.tag);
    }
    // if accidentally found during first iteration so is_first_time is true -> also replace
    if (map[cache_address.tag]->is_first_time) {
        replace_lru(address, cache_address.tag);
    }
    // write to lru and ram and update lru to mru
    Node* node = map[cache_address.tag];
    node->data[cache_address.offset] = data_to_write;       // write O(1)
    main_memory.write_to_ram(address, data_to_write);       // write-through: write directly to memory
    push_to_head(node);                                     // replace O(1)
}

void LRUCache::replace_lru(int address, int cache_address_tag) {
    // replace lru node with new node at lru place (tail)
    Node* lru_node = tail->prev;
    Node* new_node = new Node();
    new_node->next = tail;
    new_node->prev = lru_node->prev;
    lru_node->prev->next = new_node;
    tail->prev = new_node;

    // update map_key and is_first_time
    new_node->map_key = cache_address_tag;
    new_node->is_first_time = false;

    // delete entry of lrunode of map and add the new one
    map.erase(lru_node->map_key);
    map[cache_address_tag] = new_node;

    delete lru_node;

    // get appropriate 4 (number_of_offset) byte data from ram
    // 1 -> 0 1 2 3    1 / 4 = 0   0 * 4 = 0     fetch 0 - 3
    // 2 -> 0 1 2 3    2 / 4 = 0   0 * 4 = 0     fetch 0 - 3
    // 5 -> 4 5 6 7    5 / 4 = 1   1 * 4 = 4     fetch 4 - 7
    // 9 -> 8 9 10 11  9 / 4 = 2   2 * 4 = 8     fetch 8 - 11
    int start_address_to_fetch = (address / number_of_offset) * number_of_offset;
    int last_address_to_fetch = start_address_to_fetch + number_of_offset - 1;
    
    for (int ram_address = start_address_to_fetch, offset = 0; ram_address <= last_address_to_fetch; ram_address++, offset++) { // O(1)
        new_node->data[offset] = main_memory.read_from_ram(ram_address);
    }
}

int main() {
    // Create an instance of LRUCache
    LRUCache cache;

    // Test read and write operations
    cache.write_to_cache(0, 10); // Write value 10 to address 0
    cache.write_to_cache(4, 20); // Write value 20 to address 4

    int value1 = cache.read_from_cache(0); // Read value from address 0
    int value2 = cache.read_from_cache(4); // Read value from address 4

    cout << "Value at address 0: " << value1 << endl; // Expected output: 10
    cout << "Value at address 4: " << value2 << endl; // Expected output: 20

    return 0;
}