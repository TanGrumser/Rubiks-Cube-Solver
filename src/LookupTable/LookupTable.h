#pragma once

#include <string>
#include "../Model/RubicsCubeState.h"
#include <iostream>

using std::string;

namespace LookupTable {
    extern int threadCount;
    typedef uint64_t(*IndexCalculation)(RubicsCubeState&);
    typedef RubicsCubeState&(*StateCalculator)(uint64_t);
    
    const string CORNER_LOOKUP_TABLE_PATH         = "LookupTables\\cornerLookupTable";
    const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\upperEdgeLookupTable";
    const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\lowerEdgeLookupTable";
    const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigUpperEdgeLookupTable";
    const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigLowerEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string FULL_EDGE_LOOKUP_TABLE_PATH      = "fullEdgeLookupTable";

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64_t maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState& state);
    int GetUpperEdgeStateDistance(RubicsCubeState& state);
    int GetLowerEdgeStateDistance(RubicsCubeState& state);
    int GetBigUpperEdgeStateDistance(RubicsCubeState& state);
    int GetBigLowerEdgeStateDistance(RubicsCubeState& state);
    int GetEdgePermutationStateDistance(RubicsCubeState& state);
    int GetFullEdgeStateDistance(RubicsCubeState& state);
    
    // TODO comment what these do
    uint64_t GetCornerLookupIndex(RubicsCubeState& state);
    uint64_t GetUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64_t GetLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64_t GetBigUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64_t GetBigLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64_t GetEdgePermutationLookupIndex(RubicsCubeState& state);
    uint64_t GetFullEdgeLookupIndex(RubicsCubeState& state);
    
    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubicsCubeState& state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
    void GenerateEdgePermutationLookupTable();
    void GenerateFullEdgeLookupTable(string path = "");

    // TODO delete
    int GetRotationIndex(RubicsCubeState& state);
}

