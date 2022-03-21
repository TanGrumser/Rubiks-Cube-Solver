#include <iostream>
#include <string>
#include "RubicsCubeState.h"
#include "Turn.h"
#include "StringUtils.h"
#include <vector>
#include "Solver.h"
#include "Stopwatch.h"
#include "LookupTable.h"
#include <thread>
#include <mutex>
#include "FileManagement.h"
#include "CommandLineHandler.h"

int main(int argc, char *argv[]) {
    //LookupTable::LoadLookupTables();

    //Solver::GetEdgeNeighbourIndiciesRotations(RubicsCubeState::InitialState());
     CommandLineHandler::Start(argc, argv);
    //LookupTable::GenerateUpperEdgeLookupTable(LookupTable::UPPER_EDGE_LOOKUP_TABLE_PATH);
    //LookupTable::GenerateLowerEdgeLookupTable(LookupTable::LOWER_EDGE_LOOKUP_TABLE_PATH);
    
    //int maxDepth = std::stoi(argv[1]);
    

    return 0;
}