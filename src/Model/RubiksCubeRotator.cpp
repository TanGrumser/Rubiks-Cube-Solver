#include "RubiksCubeState.h"

inline void rotateCorner(RubiksCubePiece& corner, unsigned int amount) {
    corner.rotation += amount;

    // % 3, but a bit faster.
    if (corner.rotation == 3)
      corner.rotation = 0;
    else if (corner.rotation == 4)
      corner.rotation = 1;
}

inline void rotateEdge(RubiksCubePiece& edge)   {
    edge.rotation ^= 1;
}

void up(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[3];
    RubiksCubePiece savedEdge = state->edges[3];
    
    state->corners[3] = state->corners[2];
    state->corners[2] = state->corners[1];
    state->corners[1] = state->corners[0];
    state->corners[0] = savedCorner;

    state->edges[3] = state->edges[2];
    state->edges[2] = state->edges[1];
    state->edges[1] = state->edges[0];
    state->edges[0] = savedEdge;
}

void upPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[3];
    RubiksCubePiece savedEdge = state->edges[3];
    
    state->corners[3] = state->corners[0];
    state->corners[0] = state->corners[1];
    state->corners[1] = state->corners[2];
    state->corners[2] = savedCorner;

    state->edges[3] = state->edges[0];
    state->edges[0] = state->edges[1];
    state->edges[1] = state->edges[2];
    state->edges[2] = savedEdge;
}

void up2(RubiksCubeState* state) {
    std::swap(state->corners[0], state->corners[2]);
    std::swap(state->corners[1], state->corners[3]);

    std::swap(state->edges[0], state->edges[2]);
    std::swap(state->edges[1], state->edges[3]);
}

void front(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[1];
    RubiksCubePiece savedEdge = state->edges[1];
    
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
}

void frontPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[1];
    RubiksCubePiece savedEdge = state->edges[1];
    
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
}

void front2(RubiksCubeState* state) {
    std::swap(state->corners[1], state->corners[6]);
    std::swap(state->corners[2], state->corners[5]);

    std::swap(state->edges[1], state->edges[9]);
    std::swap(state->edges[6], state->edges[5]);
}

void right(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[0];
    RubiksCubePiece savedEdge = state->edges[0];
    
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

void rightPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[0];
    RubiksCubePiece savedEdge = state->edges[0];
    
    state->corners[0] = state->corners[4];
    state->corners[4] = state->corners[5];
    state->corners[5] = state->corners[1];
    state->corners[1] = savedCorner;

    rotateCorner(state->corners[0], 2);
    rotateCorner(state->corners[1], 1);
    rotateCorner(state->corners[4], 1);
    rotateCorner(state->corners[5], 2);

    state->edges[0] = state->edges[4];
    state->edges[4] = state->edges[8];
    state->edges[8] = state->edges[5];
    state->edges[5] = savedEdge;

    rotateEdge(state->edges[0]);
    rotateEdge(state->edges[4]);
    rotateEdge(state->edges[5]);
    rotateEdge(state->edges[8]);
}

void right2(RubiksCubeState* state) {
    std::swap(state->corners[1], state->corners[4]);
    std::swap(state->corners[0], state->corners[5]);

    std::swap(state->edges[0], state->edges[8]);
    std::swap(state->edges[4], state->edges[5]);
}

void back(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[0];
    RubiksCubePiece savedEdge = state->edges[3];
    
    state->corners[0] = state->corners[4];
    state->corners[4] = state->corners[7];
    state->corners[7] = state->corners[3];
    state->corners[3] = savedCorner;

    rotateCorner(state->corners[0], 1);
    rotateCorner(state->corners[3], 2);
    rotateCorner(state->corners[4], 2);
    rotateCorner(state->corners[7], 1);

    state->edges[3] = state->edges[4];
    state->edges[4] = state->edges[11];
    state->edges[11] = state->edges[7];
    state->edges[7] = savedEdge;
}

void backPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[0];
    RubiksCubePiece savedEdge = state->edges[3];
    
    state->corners[0] = state->corners[3];
    state->corners[3] = state->corners[7];
    state->corners[7] = state->corners[4];
    state->corners[4] = savedCorner;

    rotateCorner(state->corners[0], 1);
    rotateCorner(state->corners[3], 2);
    rotateCorner(state->corners[4], 2);
    rotateCorner(state->corners[7], 1);

    state->edges[3] = state->edges[7];
    state->edges[7] = state->edges[11];
    state->edges[11] = state->edges[4];
    state->edges[4] = savedEdge;
}

void back2(RubiksCubeState* state) {
    std::swap(state->corners[0], state->corners[7]);
    std::swap(state->corners[3], state->corners[4]);

    std::swap(state->edges[3], state->edges[11]);
    std::swap(state->edges[4], state->edges[7]);
}

void left(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[2];
    RubiksCubePiece savedEdge = state->edges[2];
    
    state->corners[2] = state->corners[3];
    state->corners[3] = state->corners[7];
    state->corners[7] = state->corners[6];
    state->corners[6] = savedCorner;

    rotateCorner(state->corners[2], 2);
    rotateCorner(state->corners[3], 1);
    rotateCorner(state->corners[6], 1);
    rotateCorner(state->corners[7], 2);

    state->edges[2] = state->edges[7];
    state->edges[7] = state->edges[10];
    state->edges[10] = state->edges[6];
    state->edges[6] = savedEdge;

    rotateEdge(state->edges[2]);
    rotateEdge(state->edges[6]);
    rotateEdge(state->edges[7]);
    rotateEdge(state->edges[10]);
}

void leftPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[2];
    RubiksCubePiece savedEdge = state->edges[2];
    
    state->corners[2] = state->corners[6];
    state->corners[6] = state->corners[7];
    state->corners[7] = state->corners[3];
    state->corners[3] = savedCorner;

    rotateCorner(state->corners[2], 2);
    rotateCorner(state->corners[3], 1);
    rotateCorner(state->corners[6], 1);
    rotateCorner(state->corners[7], 2);

    state->edges[2] = state->edges[6];
    state->edges[6] = state->edges[10];
    state->edges[10] = state->edges[7];
    state->edges[7] = savedEdge;

    rotateEdge(state->edges[2]);
    rotateEdge(state->edges[6]);
    rotateEdge(state->edges[7]);
    rotateEdge(state->edges[10]);
}

void left2(RubiksCubeState* state) {
    std::swap(state->corners[2], state->corners[7]);
    std::swap(state->corners[3], state->corners[6]);

    std::swap(state->edges[2], state->edges[10]);
    std::swap(state->edges[6], state->edges[7]);
}

void down(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[4];
    RubiksCubePiece savedEdge = state->edges[8];
    
    state->corners[4] = state->corners[5];
    state->corners[5] = state->corners[6];
    state->corners[6] = state->corners[7];
    state->corners[7] = savedCorner;

    state->edges[8] = state->edges[9];
    state->edges[9] = state->edges[10];
    state->edges[10] = state->edges[11];
    state->edges[11] = savedEdge;
}

void downPrime(RubiksCubeState* state) {
    RubiksCubePiece savedCorner = state->corners[4];
    RubiksCubePiece savedEdge = state->edges[8];
    
    state->corners[4] = state->corners[7];
    state->corners[7] = state->corners[6];
    state->corners[6] = state->corners[5];
    state->corners[5] = savedCorner;

    state->edges[8] = state->edges[11];
    state->edges[11] = state->edges[10];
    state->edges[10] = state->edges[9];
    state->edges[9] = savedEdge;
}

void down2(RubiksCubeState* state) {
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
void RubiksCubeState::ApplyTurn(Turn turn) {
    switch (turn.index) {
        case 0:  up(this);         break;
        case 1:  up2(this);        break;
        case 2:  upPrime(this);    break;
 
        case 3:  front(this);      break;
        case 4:  front2(this);     break;
        case 5:  frontPrime(this); break;
 
        case 6:  right(this);      break;
        case 7:  right2(this);     break;
        case 8:  rightPrime(this); break;

        case 9:  back(this);       break;
        case 10: back2(this);      break;
        case 11: backPrime(this);  break;

        case 12: left(this);       break;
        case 13: left2(this);      break;
        case 14: leftPrime(this);  break;

        case 15: down(this);       break;
        case 16: down2(this);      break;
        case 17: downPrime(this);  break;
    
        default:
            break;
    }
}
