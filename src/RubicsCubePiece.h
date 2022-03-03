#pragma once

#include <string>

using namespace std;

struct RubicsCubePiece {
    int index;
    int rotation;

    RubicsCubePiece(int index, int rotation) {
        this->index = index;
        this->rotation = rotation;
    }

    RubicsCubePiece Copy() {
        return RubicsCubePiece(index, rotation);
    }

    string ToString() {
        return to_string(index) + "," + to_string(rotation);
    }
};