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

using namespace std;

int main(int argc, char *argv[]) {
    int maxDepth = std::stoi(argv[1]);
    LookupTable::GenerateCornerLookupTable("cornerLookupTable", maxDepth);

    int* size = new int();
    char* lookupTable = FileManagement::LoadBufferFromFile("cornerLookupTable", size);

    std::vector<int> occurences(12, 0);
    std::cout << "size: " << *size << endl;
    
    for (int i = 0; i < LookupTable::CORNER_STATES_COUNT; i++) {
        if (lookupTable[i] != -1) {
            //std::cout << lookupTable[i] << endl;
            occurences[lookupTable[i]]++;
        }
    }
    
    for (int i = 0; i < 12; i++) {
        std::cout << occurences[i] << endl;
    }

    //CommandLineHandler::Start(argc, argv);

    return 0;
}