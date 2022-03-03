#include "RubicsCubeState.h"

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

/**
 * @brief Apply a turn to the current state of the rubics cube.
 * 
 * @param turn the turn, that shall be applied
 */
void RubicsCubeState::ApplyTurn(Turn turn) {
    // if we have a double turn, for now we just apply two 90° turns.
    // TODO this is quit inefficient, so this needs to be improved later on.
    if (turn.index % 3 == 1) {
        turn.index--;
        ApplyTurn(Turn(turn.index));
    }

    // Rotate the corner pieces.
    RubicsCubePiece savedPiece = cornerPieces[CORNER_PIECE_INDICIES[turn.index][3]];

    for (int i = 3; i > 0; i--) {
        cornerPieces[CORNER_PIECE_INDICIES[turn.index][i]] = cornerPieces[CORNER_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]];
        cornerPieces[CORNER_PIECE_INDICIES[turn.index][i]].rotation = 
            (cornerPieces[CORNER_PIECE_INDICIES[turn.index][i]].rotation + 3 - CORNER_ROTATION_INDICIES[turn.index][(i - 1 + 4) % 4]) % 3;
    }

    cornerPieces[CORNER_PIECE_INDICIES[turn.index][0]] = savedPiece;
    cornerPieces[CORNER_PIECE_INDICIES[turn.index][0]].rotation = 
        (cornerPieces[CORNER_PIECE_INDICIES[turn.index][0]].rotation + 3 - CORNER_ROTATION_INDICIES[turn.index][3]) % 3;

    
    // Rotate the edge pieces.
    savedPiece = edgePieces[EDGE_PIECE_INDICIES[turn.index][3]];

    for (int i = 3; i > 0; i--) {
        edgePieces[EDGE_PIECE_INDICIES[turn.index][i]] = edgePieces[EDGE_PIECE_INDICIES[turn.index][(i - 1 + 4) % 4]];
        edgePieces[EDGE_PIECE_INDICIES[turn.index][i]].rotation = 
            (edgePieces[EDGE_PIECE_INDICIES[turn.index][i]].rotation + EDGE_ROTATION_INDICIES[turn.index][(i - 1 + 4) % 4]) % 2;
    }

    edgePieces[EDGE_PIECE_INDICIES[turn.index][0]] = savedPiece;
    edgePieces[EDGE_PIECE_INDICIES[turn.index][0]].rotation = 
        (edgePieces[EDGE_PIECE_INDICIES[turn.index][0]].rotation + EDGE_ROTATION_INDICIES[turn.index][3]) % 2;
}
