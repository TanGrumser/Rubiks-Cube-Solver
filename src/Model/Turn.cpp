#include <vector>
#include <iostream>
#include "Turn.h"
#include <random>

static bool randomSeedSet = false;
static std::mt19937 gen;
static std::uniform_int_distribution<> distr;


Turn Turn::Random() {
    if (!randomSeedSet) {
        std::random_device rd; // obtain a random number from hardware
        gen = std::mt19937(rd()); // seed the generator
        distr = std::uniform_int_distribution<>(0, 17);
    }

    return Turn(distr(gen));
}

Turn Turn::Inverse() const {
    int side = index / 3;
    int rotation = index % 3;

    switch (rotation) {
        case 0: return Turn(side * 3 + 2);
        case 2: return Turn(side * 3 + 0);
        default: return Turn(side * 3 + 1);
    }
}

void Turn::Test() {
    for (Turn turn : Turn::AllTurns) {
        Turn doubleInverse = turn.Inverse().Inverse();

        if (doubleInverse.index != turn.index) {
            std::cout << "error with turn " << turn.index << endl;
            std::cout << "turn index " << turn.index << ", inverse " << turn.Inverse().index << ", doubleInverse inverse " << doubleInverse.index << endl;
            return;
        }
    }

    std::cout << "No errors found." << endl;
}

std::vector<Turn> Turn::GetSubsetTurns(int index, int parts) {
    if (Turn::AllTurns.size() % parts != 0) {
        throw std::runtime_error("Couldn't split turns into " + parts);
    }
    
    int subsetSize = Turn::AllTurns.size() / parts;
    vector<Turn> turns(subsetSize);

    for (int i = 0; i < subsetSize; i++) {
        turns[i] = Turn::AllTurns[index * subsetSize + i];
    }

    return turns;
}

const std::vector<Turn> Turn::AllTurns = {
    Turn(0),
    Turn(1),
    Turn(2),
    Turn(3),
    Turn(4),
    Turn(5),
    Turn(6),
    Turn(7),
    Turn(8),
    Turn(9),
    Turn(10),
    Turn(11),
    Turn(12),
    Turn(13),
    Turn(14),
    Turn(15),
    Turn(16),
    Turn(17)
};