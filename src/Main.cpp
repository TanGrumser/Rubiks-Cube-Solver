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
#include "LookupTable.h"
#include "CommandLineHandler.h"
#include "DuplicateState.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    DuplicateState::GenerateLookupTable();
    DuplicateState::LoadDuplicateStateIndex();
    
    //CommandLineHandler::Start(argc, argv);
    //LookupTable::GenerateBigUpperEdgeLookupTable(LookupTable::BIG_UPPER_EDGE_LOOKUP_TABLE_PATH);
    //LookupTable::GenerateBigLowerEdgeLookupTable(LookupTable::BIG_LOWER_EDGE_LOOKUP_TABLE_PATH);    

    return 0;
}