#pragma once

#define uint64 unsigned long long

struct StateIndex {
    uint64 cornerIndex;
    uint64 edgeIndex;

    bool operator==(const StateIndex &other) const { 
        return cornerIndex == other.cornerIndex && edgeIndex == other.edgeIndex;
    }
};