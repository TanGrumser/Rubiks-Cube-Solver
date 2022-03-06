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

    static Turn Empty() { return Turn(-1); }
    
    const static inline int CountAllTurns = 18;
    static Turn AllTurns[];
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
        } else if (turnString.at(0) == 'D' || turnString.at(0) == 'd') {
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

string ToString() {
        string result = "";

        int side = index / 3;
        int angle = index % 3;

        switch (side) {
            case 0: result += "U"; break;
            case 1: result += "F"; break;
            case 2: result += "R"; break;
            case 3: result += "B"; break;
            case 4: result += "L"; break;
            case 5: result += "D"; break;
        }

        switch (angle) {
            case 1: result += "2"; break;
            case 2: result += "'"; break;
        }

        return result;
    }

    bool IsTurnBacktracking(Turn lastTurn) {
        if (lastTurn.Equals(Turn::Empty())) {
            return false;
        }

        int thisSide = index / 3;
        int lastSide = lastTurn.index / 3;
        
        return 
            (thisSide == lastSide) ||
            (thisSide == 0 && lastSide == 5) || 
            (thisSide == 1 && lastSide == 3) ||
            (thisSide == 2 && lastSide == 4); 
    }

    bool Equals(Turn other) {
        return index == other.index;
    }
};

Turn inline Turn::AllTurns[] = {
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