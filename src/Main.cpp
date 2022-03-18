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
    LookupTable::LoadLookupTables();

    //Solver::GetEdgeNeighbourIndiciesRotations(RubicsCubeState::InitialState());
    CommandLineHandler::Start(argc, argv);
    /*
    int maxDepth = std::stoi(argv[1]);
    LookupTable::GenerateCornerLookupTable("cornerLookupTable", maxDepth);

    int* size = new int();
    char* lookupTable = FileManagement::LoadBufferFromFile("cornerLookupTable", size);

    std::vector<int> occurences(12, 0);
    std::cout << "size: " << *size << endl;
    
    for (int i = 0; i < *size; i++) {        
        if (lookupTable[i] != -1) {
            occurences[lookupTable[i]]++;
        }
        
    }

    int sum = 0;

    for (int i = 0; i < 12; i++) {
        std::cout << occurences[i] << endl;
        sum+= occurences[i];
    }

    std::cout << "sum: " << sum << endl;

    */

    return 0;
}