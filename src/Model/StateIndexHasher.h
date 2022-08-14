#include <iterator>
#include "StateIndex.h"
#include "../Utils/robin-map/robin_hash.h"
#include <functional>

template <class T>

inline void hash_combine(std::size_t & s, const T & v) {    
    s ^= std::hash<T>{}(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}

struct StateIndexHasher {
  std::size_t operator()(const StateIndex& k) const {
    using std::size_t;
    using std::hash;
    size_t h = 17;

    hash_combine(h, k.edgeIndex);
    hash_combine(h, k.cornerIndex);

    //return h;
    return (k.cornerIndex * k.edgeIndex) ^ (k.cornerIndex << 6) ^ (k.edgeIndex >> 2); 
  }
};
