#ifndef INT_PAIR_H
#define INT_PAIR_H

#include <queue>

template<class K, class V>
struct Pair
{
    K key;
    V value;
};

template <class PairType>
struct LessComparator
{
    inline bool operator()(PairType left, PairType right)
    {
        return left.key < right.key;
    }
};

template <class PairType>
struct GreaterComparator
{
    inline bool operator()(PairType left, PairType right)
    {
        return left.key > right.key;
    }
};

// We need c++11 to cleanly define this
// std::priority_queue requires 3 types as template parameters
// first parameter is the actual type "T"
// second parameter is the underlying container (defaulted to "std::vector<T>")
// last parameter comparison "functor" (function object).
// This is defaulted to "less" (directly utilizes operator< of the type if available)
//
// Here we are morphing a template class PairHeap with two types K and V.
//
// USAGE:
//
// MaxPairHeap<int, float> name;
//
// This creates a prioirty_queue using the "int" type and it is a max heap.
// float type is accompanies the key value ("int") which will be usefull on your
// implementations.
template<class K, class V>
using MaxPairHeap = std::priority_queue<Pair<K,V>,
                                        std::vector<Pair<K,V>>,
                                        LessComparator<Pair<K,V>>>;

template<class K, class V>
using MinPairHeap = std::priority_queue<Pair<K,V>,
                                        std::vector<Pair<K,V>>,
                                        GreaterComparator<Pair<K,V>>>;

// Non-c++11 way, you can only "typedef" concrete types
// This is given as an example, this will define a priority_queue type
// that holds pairs of floats and integers
// Comparisons are done using floats, and comparison will return the lesser of the two pairs.
// Due to definition of priority_queue this will create a max heap
typedef std::priority_queue<Pair<float,int>, std::vector<Pair<float,int>>,
                            LessComparator<Pair<float,int>>> MinPairHeapFloatInt;

// You can use either way or completely new way on your implementations

#endif // INT_PAIR_H