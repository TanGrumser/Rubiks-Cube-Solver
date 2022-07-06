#pragma once

#include <string>

struct StateIndex {
    uint64_t cornerIndex;
    uint64_t edgeIndex;

    bool operator==(const StateIndex &other) const { 
        return cornerIndex == other.cornerIndex && edgeIndex == other.edgeIndex;
    }
    
    string toString() const {
        return std::to_string(cornerIndex) + " " + std::to_string(edgeIndex);
    }
};