#include "RubicsCubeState.h"
#include <iostream>

/**
 * @brief the indicies of the corners of a side (index / 3).
 * 
 */
const int CORNER_PIECE_INDICIES[18][4] = {
    // upper side
    {0, 1, 2, 3}, // 90°
    {0, 1, 2, 3}, // 180°
    {3, 2, 1, 0}, // 270°
    // front side
    {2, 1, 5, 6}, // 90°
    {2, 1, 5, 6}, // 180°
    {6, 5, 1, 2}, // 270°
    // right side
    {0, 4, 5, 1}, // 90°
    {0, 4, 5, 1}, // 180°
    {1, 5, 4, 0}, // 270°
    // back side
    {3, 7, 4, 0}, // 90°
    {3, 7, 4, 0}, // 180°
    {0, 4, 7, 3}, // 270°
    // left side
    {3, 2, 6, 7}, // 90°
    {3, 2, 6, 7}, // 180°
    {7, 6, 2, 3}, // 270°
    // down side
    {4, 7, 6, 5}, // 90°
    {4, 7, 6, 5}, // 180°
    {5, 6, 7, 4}  // 270°
};

const int CORNER_ROTATION_INDICIES[18][4] = {
    // upper side
    {0, 0, 0, 0}, // 90°
    {0, 0, 0, 0}, // 180°
    {0, 0, 0, 0}, // 270°
    // front side
    {1, 2, 1, 2}, // 90°
    {1, 2, 1, 2}, // 180°
    {2, 1, 2, 1}, // 270°
    // right side
    {2, 1, 2, 1}, // 90°
    {2, 1, 2, 1}, // 180°
    {1, 2, 1, 2}, // 270°
    // back side
    {2, 1, 2, 1}, // 90°
    {2, 1, 2, 1}, // 180°
    {1, 2, 1, 2}, // 270°
    // left side
    {1, 2, 1, 2}, // 90°
    {1, 2, 1, 2}, // 180°
    {2, 1, 2, 1}, // 270°
    // down side
    {0, 0, 0, 0}, // 90°
    {0, 0, 0, 0}, // 180°
    {0, 0, 0, 0}  // 270°
};

const int EDGE_PIECE_INDICIES[18][4] = {
    // upper side
    {0, 1, 2, 3}, // 90°
    {0, 1, 2, 3}, // 180°
    {3, 2, 1, 0}, // 270°
    // front side
    {1, 5, 9, 6}, // 90°
    {1, 5, 9, 6}, // 180°
    {6, 9, 5, 1}, // 270°
    // right side
    {0, 4, 8, 5}, // 90°
    {0, 4, 8, 5}, // 180°
    {5, 8, 4, 0}, // 270°
    // back side
    {3, 7, 11, 4}, // 90°
    {3, 7, 11, 4}, // 180°
    {4, 11, 7, 3}, // 270°
    // left side
    {2, 6, 10, 7}, // 90°
    {2, 6, 10, 7}, // 180°
    {7, 10, 6, 2}, // 270°
    // down side
    {9, 8, 11, 10}, // 90°
    {9, 8, 11, 10}, // 180°
    {10, 11, 8, 9}  // 270°
};

const int EDGE_ROTATION_INDICIES[18][4] = {
    // upper side
    {0, 0, 0, 0}, // 90°
    {0, 0, 0, 0}, // 180°
    {0, 0, 0, 0}, // 270°
    // front side
    {0, 0, 1, 1}, // 90°
    {0, 0, 1, 1}, // 180°
    {1, 0, 0, 1}, // 270°
    // right side
    {0, 0, 1, 1}, // 90°
    {0, 0, 1, 1}, // 180°
    {1, 0, 0, 1}, // 270°
    // back side
    {0, 0, 1, 1}, // 90°
    {0, 0, 1, 1}, // 180°
    {1, 0, 0, 1}, // 270°
    // left side
    {0, 0, 1, 1}, // 90°
    {0, 0, 1, 1}, // 180°
    {1, 0, 0, 1}, // 270°
    // down side
    {0, 0, 0, 0}, // 90°
    {0, 0, 0, 0}, // 180°
    {0, 0, 0, 0} // 270°
};

inline void rotateCorner(RubicsCubePiece& corner, unsigned int amount) {
    corner.rotation += amount;

    // % 3, but a bit faster.
    if (corner.rotation == 3)
      corner.rotation = 0;
    else if (corner.rotation == 4)
      corner.rotation = 1;
}

inline void rotateEdge(RubicsCubePiece& edge)   {
    edge.rotation ^= 1;
}

void up(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[3];
    RubicsCubePiece savedEdge = state->edges[3];
    
    state->corners[3] = state->corners[2];
    state->corners[2] = state->corners[1];
    state->corners[1] = state->corners[0];
    state->corners[0] = savedCorner;

    state->edges[3] = state->edges[2];
    state->edges[2] = state->edges[1];
    state->edges[1] = state->edges[0];
    state->edges[0] = savedEdge;

    rotateEdge(state->edges[0]);
    rotateEdge(state->edges[1]);
    rotateEdge(state->edges[2]);
    rotateEdge(state->edges[3]);
}

void upPrime(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[3];
    RubicsCubePiece savedEdge = state->edges[3];
    
    state->corners[3] = state->corners[0];
    state->corners[0] = state->corners[1];
    state->corners[1] = state->corners[2];
    state->corners[2] = savedCorner;

    state->edges[3] = state->edges[0];
    state->edges[0] = state->edges[1];
    state->edges[1] = state->edges[2];
    state->edges[2] = savedEdge;

    rotateEdge(state->edges[0]);
    rotateEdge(state->edges[1]);
    rotateEdge(state->edges[2]);
    rotateEdge(state->edges[3]);
}

void up2(RubicsCubeState* state) {
    std::swap(state->corners[0], state->corners[2]);
    std::swap(state->corners[1], state->corners[3]);

    std::swap(state->edges[0], state->edges[2]);
    std::swap(state->edges[1], state->edges[3]);
}

void front(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[1];
    RubicsCubePiece savedEdge = state->edges[1];
    
    state->corners[1] = state->corners[2];
    state->corners[2] = state->corners[6];
    state->corners[6] = state->corners[5];
    state->corners[5] = savedCorner;

    rotateCorner(state->corners[1], 2);
    rotateCorner(state->corners[2], 1);
    rotateCorner(state->corners[5], 1);
    rotateCorner(state->corners[6], 2);

    state->edges[1] = state->edges[6];
    state->edges[6] = state->edges[9];
    state->edges[9] = state->edges[5];
    state->edges[5] = savedEdge;

    rotateEdge(state->edges[1]);
    rotateEdge(state->edges[5]);
    rotateEdge(state->edges[6]);
    rotateEdge(state->edges[9]);
}

void frontPrime(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[1];
    RubicsCubePiece savedEdge = state->edges[1];
    
    state->corners[1] = state->corners[5];
    state->corners[5] = state->corners[6];
    state->corners[6] = state->corners[2];
    state->corners[2] = savedCorner;

    rotateCorner(state->corners[1], 2);
    rotateCorner(state->corners[2], 1);
    rotateCorner(state->corners[5], 1);
    rotateCorner(state->corners[6], 2);

    state->edges[1] = state->edges[5];
    state->edges[5] = state->edges[9];
    state->edges[9] = state->edges[6];
    state->edges[6] = savedEdge;

    rotateEdge(state->edges[1]);
    rotateEdge(state->edges[5]);
    rotateEdge(state->edges[6]);
    rotateEdge(state->edges[9]);
}

void front2(RubicsCubeState* state) {
    std::swap(state->corners[1], state->corners[6]);
    std::swap(state->corners[2], state->corners[5]);

    std::swap(state->edges[1], state->edges[9]);
    std::swap(state->edges[6], state->edges[5]);
}

void right(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[0];
    RubicsCubePiece savedEdge = state->edges[0];
    
    state->corners[0] = state->corners[1];
    state->corners[1] = state->corners[5];
    state->corners[5] = state->corners[4];
    state->corners[4] = savedCorner;

    rotateCorner(state->corners[0], 2);
    rotateCorner(state->corners[1], 1);
    rotateCorner(state->corners[4], 1);
    rotateCorner(state->corners[5], 2);

    state->edges[0] = state->edges[5];
    state->edges[5] = state->edges[8];
    state->edges[8] = state->edges[4];
    state->edges[4] = savedEdge;

    rotateEdge(state->edges[0]);
    rotateEdge(state->edges[4]);
    rotateEdge(state->edges[5]);
    rotateEdge(state->edges[8]);
}

void rightPrime(RubicsCubeState* state) {
    RubicsCubePiece savedCorner = state->corners[1];
    RubicsCubePiece savedEdge = state->edges[1];
    
    state->corners[1] = state->corners[5];
    state->corners[5] = state->corners[6];
    state->corners[6] = state->corners[2];
    state->corners[2] = savedCorner;

    rotateCorner(state->corners[1], 2);
    rotateCorner(state->corners[2], 1);
    rotateCorner(state->corners[5], 1);
    rotateCorner(state->corners[6], 2);

    state->edges[1] = state->edges[5];
    state->edges[5] = state->edges[9];
    state->edges[9] = state->edges[6];
    state->edges[6] = savedEdge;

    rotateEdge(state->edges[1]);
    rotateEdge(state->edges[5]);
    rotateEdge(state->edges[6]);
    rotateEdge(state->edges[9]);
}
void right2(RubicsCubeState* state) {
    std::swap(state->corners[1], state->corners[4]);
    std::swap(state->corners[0], state->corners[5]);

    std::swap(state->edges[0], state->edges[8]);
    std::swap(state->edges[4], state->edges[5]);
}

void back2(RubicsCubeState* state) {
    std::swap(state->corners[0], state->corners[7]);
    std::swap(state->corners[3], state->corners[4]);

    std::swap(state->edges[3], state->edges[11]);
    std::swap(state->edges[4], state->edges[7]);
}

void left2(RubicsCubeState* state) {
    std::swap(state->corners[2], state->corners[7]);
    std::swap(state->corners[3], state->corners[6]);

    std::swap(state->edges[2], state->edges[10]);
    std::swap(state->edges[6], state->edges[7]);
}

void down2(RubicsCubeState* state) {
    std::swap(state->corners[4], state->corners[6]);
    std::swap(state->corners[5], state->corners[7]);

    std::swap(state->edges[8], state->edges[10]);
    std::swap(state->edges[9], state->edges[11]);
}

/**
 * @brief Apply a turn to the current state of the rubics cube.
 * 
 * @param turn the turn, that shall be applied
 */
void RubicsCubeState::ApplyTurn(Turn turn) {
    switch (turn.index) {
        case 0: up(this);         return;
        case 1: up2(this);        return;
        case 2: upPrime(this);    return;

        case 3: front(this);      return;
        case 4: front2(this);     return;
        case 5: frontPrime(this); return;

        case 6: right(this);      return;
        case 7: right2(this);     return;
        case 8: rightPrime(this); return;

        case 10: back2(this); return;
        case 13: left2(this); return;
        case 16: down2(this); return;
    
        default:
            break;
    }

    // if we have a double turn, for now we just apply two 90° turns.
    // TODO this is quit inefficient, so this needs to be improved later on.
    if (turn.index % 3 == 1) {
        turn.index--;
        ApplyTurn(Turn(turn.index));
    }

    // Rotate the corner pieces.
    RubicsCubePiece savedCorner = corners[CORNER_PIECE_INDICIES[turn.index][3]];

    for (int i = 3; i > 0; i--) {
        corners[CORNER_PIECE_INDICIES[turn.index][i]].index = corners[CORNER_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]].index;
        corners[CORNER_PIECE_INDICIES[turn.index][i]].rotation = (corners[CORNER_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]].rotation + 3 - CORNER_ROTATION_INDICIES[turn.index][(i - 1 + 4) % 4]) % 3;
    }

    corners[CORNER_PIECE_INDICIES[turn.index][0]] = savedCorner;
    corners[CORNER_PIECE_INDICIES[turn.index][0]].rotation = (savedCorner.rotation + 3 - CORNER_ROTATION_INDICIES[turn.index][3]) % 3;

    
    // Rotate the edge pieces.
    RubicsCubePiece savedEdge = edges[EDGE_PIECE_INDICIES[turn.index][3]];

    for (int i = 3; i > 0; i--) {
        edges[EDGE_PIECE_INDICIES[turn.index][i]].index = edges[EDGE_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]].index;
        edges[EDGE_PIECE_INDICIES[turn.index][i]].rotation = (edges[EDGE_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]].rotation + EDGE_ROTATION_INDICIES[turn.index][(i - 1 + 4) % 4]) % 2;
    }

    edges[EDGE_PIECE_INDICIES[turn.index][0]] = savedEdge;
    edges[EDGE_PIECE_INDICIES[turn.index][0]].rotation = (savedEdge.rotation + EDGE_ROTATION_INDICIES[turn.index][3]) % 2;
}
