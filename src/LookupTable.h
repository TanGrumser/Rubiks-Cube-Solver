#pragma once

#include <string>
#include "RubicsCubeState.h"
#include "Definitions.h"

using std::string;

namespace LookupTable {
    extern int threadCount;
    typedef uint64(*IndexCalculation)(RubicsCubeState&);
    
    const string CORNER_LOOKUP_TABLE_PATH         = "LookupTables\\cornerLookupTable";
    const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\upperEdgeLookupTable";
    const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\lowerEdgeLookupTable";
    const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigUpperEdgeLookupTable";
    const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigLowerEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string FULL_EDGE_LOOKUP_TABLE_PATH      = "fullEdgeLookupTable";

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64 maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState& state);
    int GetUpperEdgeStateDistance(RubicsCubeState& state);
    int GetLowerEdgeStateDistance(RubicsCubeState& state);
    int GetBigUpperEdgeStateDistance(RubicsCubeState& state);
    int GetBigLowerEdgeStateDistance(RubicsCubeState& state);
    int GetEdgePermutationStateDistance(RubicsCubeState& state);
    int GetFullEdgeStateDistance(RubicsCubeState& state);
    
    // TODO comment what these do
    uint64 GetCornerLookupIndex(RubicsCubeState& state);
    uint64 GetUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetBigUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetBigLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetEdgePermutationLookupIndex(RubicsCubeState& state);
    uint64 GetFullEdgeLookupIndex(RubicsCubeState& state);
    
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

