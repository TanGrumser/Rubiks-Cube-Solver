#pragma once

#include <string>

using namespace std;

struct RubiksCubePiece {
    unsigned int index;
    unsigned int rotation;

    RubiksCubePiece() {
        this->index = 0;
        this->rotation = 0;
    }

    RubiksCubePiece(unsigned int index, unsigned int rotation) {
        this->index = index;
        this->rotation = rotation;
    }

    RubiksCubePiece Copy() {
        return RubiksCubePiece(index, rotation);
    }

    string ToString() {
        return to_string(index) + "," + to_string(rotation);
    }
};