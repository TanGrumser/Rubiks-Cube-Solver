#include <vector>

#define uint64 unsigned long long int

namespace TurnIndexer {
    const int MOVE_COUNT = 18;

    uint64 getIndex(int* turnIndicies, int length);
    uint64 getIndexFromVector(vector<Turn> turns);
}