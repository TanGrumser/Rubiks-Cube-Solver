#pragma once

#include <string>

using namespace std;

struct RubicsCubePiece {
    unsigned int index;
    unsigned int rotation;

    RubicsCubePiece() {
        this->index = 0;
        this->rotation = 0;
    }

    RubicsCubePiece(unsigned int index, unsigned int rotation) {
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