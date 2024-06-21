#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <cstdint>
#include <cstddef>
#include <unordered_map>

using namespace std;

struct Request {
    uint32_t addr;
    uint32_t data;
    int we;         // 0 for read, 1 for write
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

class LRU {
private:
    struct Node {
        int key, value;
        Node* next;
        Node* prev;
        Node(int k, int v) : key(k), value(v), next(nullptr), prev(nullptr) {}
    };

    unordered_map<int, Node*> map;
    int capacity;
    Node* head;
    Node* tail;

    void pushToHead(Node* node) {
        remove(node);
        add(node);
    }

    void add(Node* node) {
        node->next = head->next;
        node->next->prev = node;
        node->prev = head;
        head->next = node;
    }

    void remove(Node* node) {
        Node* prev = node->prev;
        Node* next = node->next;
        prev->next = next;
        next->prev = prev;
    }

public:
    LRU(int capacity) : capacity(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    ~LRU() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    int get(int key) {
        if (map.find(key) == map.end()) {
            return -1;
        }
        Node* node = map[key];
        pushToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            Node* node = map[key];
            node->value = value;
            pushToHead(node);
        } else {
            if (map.size() >= capacity) {
                Node* lru = tail->prev;
                map.erase(lru->key);
                remove(lru);
                delete lru;
            }
            Node* node = new Node(key, value);
            map[key] = node;
            add(node);
        }
    }
};
#endif