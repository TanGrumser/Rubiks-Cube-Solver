#pragma once

#include <string>
#include "RubicsCubeState.h"

using std::string;

#define uint64 unsigned long long

namespace LookupTable {
    extern int threadCount;
    typedef uint64(*IndexCalculation)(RubicsCubeState*);
    
    #if DEBUG
        const string CORNER_LOOKUP_TABLE_PATH         = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\cornerLookupTable";
        const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\upperEdgeLookupTable";
        const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\lowerEdgeLookupTable";
        const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\bigUpperEdgeLookupTable";
        const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\bigLowerEdgeLookupTable";
        const string FULL_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\fullEdgeLookupTable";
    #else
        const string CORNER_LOOKUP_TABLE_PATH         = "cornerLookupTable";
        const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "upperEdgeLookupTable";
        const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "lowerEdgeLookupTable";
        const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "bigUpperEdgeLookupTable";
        const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "bigLowerEdgeLookupTable";
        const string FULL_EDGE_LOOKUP_TABLE_PATH = "fullEdgeLookupTable";
    #endif

    const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
    const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
    const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

    const int EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2; // 2^6
    const int EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7; // 12! / 6!
    const int EDGE_STATES_COUNT = EDGE_ROTATION_COUNT * EDGE_PERMUTATIONS_COUNT; // (12! / 6!) * 2^6
    
    const int BIG_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^7
    const int BIG_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6; // 12! / 5!
    const int BIG_EDGE_STATES_COUNT = BIG_EDGE_ROTATION_COUNT * BIG_EDGE_PERMUTATIONS_COUNT; // (12! / 5!) * 2^7
    
    const uint64 FULL_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^11
    const uint64 FULL_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 12! / 2!
    const uint64 FULL_EDGE_STATES_COUNT = FULL_EDGE_ROTATION_COUNT * FULL_EDGE_PERMUTATIONS_COUNT; // (12! / 2!) * 2^11

    const unsigned long long ALL_STATES_COUNT = FULL_EDGE_STATES_COUNT * CORNER_STATES_COUNT;

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64 maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState* state);
    int GetUpperEdgeStateDistance(RubicsCubeState* state);
    int GetLowerEdgeStateDistance(RubicsCubeState* state);
    int GetBigUpperEdgeStateDistance(RubicsCubeState* state);
    int GetBigLowerEdgeStateDistance(RubicsCubeState* state);
    int GetFullEdgeStateDistance(RubicsCubeState* state);
    
    // TODO comment what these do
    uint64 GetCornerLookupIndex(RubicsCubeState* state);
    uint64 GetUpperEdgeLookupIndex(RubicsCubeState* state);
    uint64 GetLowerEdgeLookupIndex(RubicsCubeState* state);
    uint64 GetBigUpperEdgeLookupIndex(RubicsCubeState* state);
    uint64 GetBigLowerEdgeLookupIndex(RubicsCubeState* state);
    uint64 GetFullEdgeLookupIndex(RubicsCubeState* state);
    
    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubicsCubeState* state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
    void GenerateFullEdgeLookupTable(string path = "");

    // TODO delete
    int GetRotationIndex(RubicsCubeState* state);
}

