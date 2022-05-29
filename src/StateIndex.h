#pragma once

#include <string>

#define uint64 unsigned long long

struct StateIndex {
    uint64 cornerIndex;
    uint64 edgeIndex;

    bool operator==(const StateIndex &other) const { 
        return cornerIndex == other.cornerIndex && edgeIndex == other.edgeIndex;
    }

    string toString() const {
        return std::to_string(cornerIndex) + " " + std::to_string(edgeIndex);
    }
};