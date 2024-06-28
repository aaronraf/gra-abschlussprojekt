#ifndef CACHETEMPLATE_HPP
#define CACHETEMPALTE_HPP

// FOR POLYMORPHISM
class Cache {
public:
    virtual int read_from_cache(int address) = 0;
    virtual void write_to_cache(int address, int data_to_write) = 0;
};

#endif