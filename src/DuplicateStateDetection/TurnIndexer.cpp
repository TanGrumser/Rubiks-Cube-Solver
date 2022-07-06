#include "TurnIndexer.h"

const uint64_t powerOfEighteen[9] = {
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

uint64_t TurnIndexer::getIndex(int* turnIndicies, int length) {
    uint64_t result = 0;

    for (int i = 0; i < length; i++) {
        result += (turnIndicies[i] + 1) * powerOfEighteen[length - i - 1];
    }

    return result;
}

uint64_t TurnIndexer::getIndexFromVector(vector<Turn> turns) {
    int* turnIndicies = (int*) malloc(sizeof(int) * turns.size());

    for (int i = 0; i < turns.size(); i++) {
        turnIndicies[i] = turns[i].index;
    }

    return getIndex(turnIndicies, turns.size());
}

uint64_t TurnIndexer::getIndexFromArray(array<Turn, 50> turns, int length) {
    uint64_t result = 0;
    int i = 0;
    
    while (!turns[i].Equals(Turn::Empty())) {
        result += (turns[i].index + 1) * powerOfEighteen[length - i - 1];
        i++;
    }

    return result;
}