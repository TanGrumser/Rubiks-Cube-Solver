#include "TurnIndexer.h"

const uint64 powerOfEighteen[9] = {
    1,
    18ull, 
    18ull * 18ull,
    18ull * 18ull * 18ull,
    18ull * 18ull * 18ull * 18ull,
    18ull * 18ull * 18ull * 18ull * 18ull,
    18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
    18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
    18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
};

uint64 TurnIndexer::getIndex(int* turnIndicies, int length) {
    uint64 result = 0;

    for (int i = 0; i < length; i++) {
        result += (turnIndicies[i] + 1) * powerOfEighteen[length - i - 1];
    }

    return result;
}

uint64 TurnIndexer::getIndexFromVector(vector<Turn> turns) {
    int turnIndicies[turns.size()];

    for (int i = 0; i < turns.size(); i++) {
        turnIndicies[i] = turns[i].index;
    }

    return getIndex(turnIndicies, turns.size());
}