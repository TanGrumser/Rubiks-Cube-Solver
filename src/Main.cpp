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
#include "RubicsCubeStateShift.h"
#include "PermutationIndexer.h"
#include <vector>
#include <random>
#include "TurnIndexer.h"
#include "StateIndexReverser.h"

int main(int argc, char *argv[]) {
    
    StateIndexReverser::Test();
    //CommandLineHandler::Start(argc, argv);

    return 0;
}