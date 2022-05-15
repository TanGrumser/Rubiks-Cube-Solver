#include <iterator>
#include "StateIndex.h"

struct StateIndexHasher {
  std::size_t operator()(const StateIndex& k) const {
    using std::size_t;
    using std::hash;

    return (k.cornerIndex * k.edgeIndex) ^ k.cornerIndex  ^ k.edgeIndex; 
  }
};