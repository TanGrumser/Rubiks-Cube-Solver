#pragma once

#include <string>
#include "../Model/RubiksCubeState.h"
#include <iostream>

using std::string;

namespace LookupTable {
    extern int threadCount;
    typedef uint64_t(*IndexCalculation)(RubiksCubeState&);
    typedef RubiksCubeState&(*StateCalculator)(uint64_t);
    
    const string CORNER_LOOKUP_TABLE_PATH         = "LookupTables\\cornerLookupTable";
    const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\upperEdgeLookupTable";
    const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\lowerEdgeLookupTable";
    const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigUpperEdgeLookupTable";
    const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigLowerEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string FULL_EDGE_LOOKUP_TABLE_PATH      = "fullEdgeLookupTable";

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64_t maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubiksCubeState& state);
    int GetUpperEdgeStateDistance(RubiksCubeState& state);
    int GetLowerEdgeStateDistance(RubiksCubeState& state);
    int GetBigUpperEdgeStateDistance(RubiksCubeState& state);
    int GetBigLowerEdgeStateDistance(RubiksCubeState& state);
    int GetEdgePermutationStateDistance(RubiksCubeState& state);
    int GetFullEdgeStateDistance(RubiksCubeState& state);
    
    // TODO comment what these do
    uint64_t GetCornerLookupIndex(RubiksCubeState& state);
    uint64_t GetUpperEdgeLookupIndex(RubiksCubeState& state);
    uint64_t GetLowerEdgeLookupIndex(RubiksCubeState& state);
    uint64_t GetBigUpperEdgeLookupIndex(RubiksCubeState& state);
    uint64_t GetBigLowerEdgeLookupIndex(RubiksCubeState& state);
    uint64_t GetEdgePermutationLookupIndex(RubiksCubeState& state);
    uint64_t GetFullEdgeLookupIndex(RubiksCubeState& state);
    
    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubiksCubeState& state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
    void GenerateEdgePermutationLookupTable();
    void GenerateFullEdgeLookupTable(string path = "");

    // TODO delete
    int GetRotationIndex(RubiksCubeState& state);
}

