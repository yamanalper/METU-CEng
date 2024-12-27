#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <cstdio>
#include "IntPair.h"
#include "Exceptions.h"

// Sentinel for probing
#define SENTINEL_MARK 0xFFFFFFFF
#define EMPTY_MARK    0xFFFFFFFE
#define OCCUPIED_MARK 0x00000000
// Capacity threshold is the multiplicative inverse of the 1/2 (%50)
#define CAPACITY_THRESHOLD 2

struct HashData
{
    // Data
    std::vector<int> intArray;
    unsigned int     sentinel;
    // Key
    bool             isCostWeighted;
    int              startInt;
    int              endInt;
    // LRU Counter (to determine least recently used entry)
    int              lruCounter;
};

template <int MAX_SIZE>
class HashTable
{
    private:
    static int  PRIMES[3];

    // Properties
    HashData    table[MAX_SIZE];
    int         elementCount;

    // Private Members
    static int  Hash(int startInt, int endInt, bool isCostWeighted);
    // Implemented Private Members
    void        PrintLine(int tableIndex) const;

    public:
    // Constructors & Destructor
                HashTable();
    // Member Functions
    int         Insert(const std::vector<int>& intArray, bool isCostWeighted);
    bool        Find(std::vector<int>& intArray,
                     int startInt, int endInt, bool isCostWeighted,
                     bool incLRU = false);
    void        Remove(std::vector<int>& intArray,
                       int startInt, int endInt, bool isCostWeighted);
    void        RemoveLRU(int lruElementCount);

    void        InvalidateTable();
    void        GetMostInserted(std::vector<int>& intArray) const;
    void        PrintSortedLRUEntries() const;
    int         lruElementCount();

    // Implemented Functions
    void        PrintTable() const;
    
};

// Template Implementation
#include "HashTableImpl.h"

#endif // HASH_TABLE_H