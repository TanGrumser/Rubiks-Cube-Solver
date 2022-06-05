#pragma once

#include <string>
#include "Stopwatch.h"
#include "RubicsCubeState.h"
#include "PermutationIndexer.h"
#include "RubicsCubeStateShift.h"

typedef void(*TestFunction)(int iterations);

int GetPermutationIndex2(RubicsCubeState* state) {
    int result = 0;

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7, 
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1
    };

    for (int i = 0; i < 8 - 1; i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 8; j++) {
            if (state->cornerPermutaion[j] == i) {
                result += position * coefficients[i];
                break;
            }
            
            if (i < state->cornerPermutaion[j]) {
                position++;
            }
        }
    }

    return result;
}

int GetEdgePermutationIndex(RubicsCubeState* state) {
    int result = 0;

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8,
        1 * 2 * 3 * 4 * 5 * 6 * 7,
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1
    };

    // We iterating only to the third-last element, since the position of the last two pieces are completely defined by the positions of all other pieces.
    for (int i = 0; i < 12 - 1; i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 12; j++) {
            if (state->edgePermutaion[j] == i) {
                result += position * coefficients[i];
                break;
            }
            
            if (i < state->edgePermutaion[j]) {
                position++;
            }
        }
    }

    return result;
}

double TestPerformanceOfFunction(TestFunction testFunction, string decription, int iterations) {
    Stopwatch::StartTimer();
    testFunction(iterations);
    Stopwatch::StopTimer();
    

    std::cout << decription << " took " << Stopwatch::GetFormattedTimeInSeconds() << endl;
    return Stopwatch::GetTimeInMilliseconds();
}

void TestPerformance(int iterations) {
    double millisOld = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

        int s;

        for (int i = 0; i < iterations; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += GetPermutationIndex2(state);
        }

        std::cout << s  << endl;
        delete state;
    }, "Calculating corner lexicographic rank one billion times with old method", iterations);
    
    double millisNew = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

        PermutationIndexer<8> cornerIndexer;
        int s;

        for (int i = 0; i < iterations; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += cornerIndexer.rank(state->cornerPermutaion);
        }
        
        std::cout << s  << endl;
        delete state;
    }, "Calculating corner lexicographic rank one billion times with new method", iterations);
    
    std::cout << "Corner performance ration of old/new = " << millisOld / millisNew << endl;

    millisOld = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

        int s;

        for (int i = 0; i < iterations; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += GetEdgePermutationIndex(state);
        }

        std::cout << s  << endl;
        delete state;
    }, "Calculating edge lexicographic rank one billion times with old method", iterations);

    millisNew = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

        PermutationIndexer<12> edgeIndexer;
        int s;

        for (int i = 0; i < iterations; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += edgeIndexer.rank(state->edgePermutaion);
        }
        
        std::cout << s  << endl;
        delete state;
    }, "Calculating edge lexicographic rank one billion times with new method", iterations);

    millisNew = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

        int s;

        for (int i = 0; i < iterations; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += state->GetLookupIndex().cornerIndex;
        }
        
        std::cout << s  << endl;
        delete state;
    }, "Calculate regular state index", iterations);

    millisNew = TestPerformanceOfFunction([](int iterations) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();
        RubicsCubeStateShift shift(state);

        int s;

        for (int i = 0; i < iterations * 10; i++) {
            // we make some arbitrary changes to the state to trick the compiler not to optimize the code away.
            state->cornerPermutaion[0] = 0;
            s += shift.GetShiftedState(state).cornerIndex;
        }
        
        std::cout << s  << endl;
        delete state;
    }, "Calculate shifted state index", iterations);

    std::cout << "Corner performance ration of old/new = " << millisOld / millisNew << endl;
}

void TestStateShift() {
        const int tries = 1000;
    int equals = 0;

    for (int j = 0; j < tries; j++) {

        RubicsCubeState* normalState = RubicsCubeState::InitialState()->Copy();
        RubicsCubeState* shuffledState = RubicsCubeState::InitialState()->Copy();

        // shuffle
        for (int i = 0; i < 20; i++) {
            Turn randomTurn = Turn::Random();
            shuffledState->ApplyTurn(randomTurn);
        }

        RubicsCubeStateShift shift(shuffledState);

        StateIndex index1 = shift.GetShiftedState(shuffledState);
        StateIndex index2 = normalState->GetLookupIndex();

        //std::cout << (index1 == index2 ? "EQUAL  " : "UNEQUAL") << " Initial: " << index1.toString() << " and should be " << index2.toString() << endl;

        for (int i = 0; i < 20; i++) {
            Turn randomTurn = Turn::Random();
            shuffledState->ApplyTurn(randomTurn);
            normalState->ApplyTurn(randomTurn);
        }

        index1 = shift.GetShiftedState(shuffledState);
        index2 = normalState->GetLookupIndex();

        equals += index1 == index2 ? 1 : 0;
    }
    
    std::cout << (equals == tries ? "Success" : "Failure") << endl;

}