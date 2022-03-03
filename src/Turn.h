#pragma once

#include <string>

using namespace std;

/**
 * Internal representation of a turn.
 * The side that is turned equals index / 3;
 * The angles by how much the side is turned can be calculated by 
 *     index % 3 == 0 -> 90°
 *     index % 3 == 1 -> 180°
 *     index % 3 == 2 -> 270°
 */
struct Turn {
    int index;

    Turn(int index) {
        this->index = index;
    }
    
    /**
     * Convert a string representing a turn to the internal representation of a turn.
     * String must follow the format [side]('/2) e.g. "L", "l", "U2", "b'", "L'" or "r2"
     * 
     * @param turnString the string representation of the turn.
     * @return internal representation of the turn.
     */
    static Turn Parse(string turnString) {
        int side, index;

        if (turnString.at(0) == 'U' || turnString.at(0) == 'u') {
            side = 0;
        } else if (turnString.at(0) == 'F' || turnString.at(0) == 'f') {
            side = 1;
        } else if (turnString.at(0) == 'R' || turnString.at(0) == 'r') {
            side = 2;
        } else if (turnString.at(0) == 'B' || turnString.at(0) == 'b') {
            side = 3;
        } else if (turnString.at(0) == 'L' || turnString.at(0) == 'l') {
            side = 4;
        } else if (turnString.at(0) == 'B' || turnString.at(0) == 'd') {
            side = 5;
        }

        index = side * 3;

        if (turnString.length() > 1) {
            if (turnString.at(1) == '2') {
                index += 1;
            } else if (turnString.at(1) == '\'') {
                index += 2;
            }
        }

        return Turn(index);
    }
};