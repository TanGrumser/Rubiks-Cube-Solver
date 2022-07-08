cmake_minimum_required(VERSION 1.0)
project(RubiksCubeSolver)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

# All source files.
set(SRC
  "src/Main.cpp"
  "../src/CommandLineHandler.h"
  "../src/CommandLineHandler.cpp"
  "../src/Model/RubiksCubePiece.h"
  "../src/Model/RubicsCubeRotator.cpp"
  "../src/Model/RubiksCubeState.h"
  "../src/Model/RubiksCubeState.cpp"
  "../src/Model/RubiksCubeStateShift.h"
  "../src/Model/StateIndex.h"
  "../src/Model/StateIndexHasher.h"
  "../src/Model/Turn.h"
  "../src/Model/Turn.cpp"
  "../src/DuplicateStateDetection/DuplicateState.h"
  "../src/DuplicateStateDetection/DuplicateState.cpp"
  "../src/DuplicateStateDetection/DuplicateStateTurnIndex.cpp"
  "../src/DuplicateStateDetection/TurnIndexer.cpp"
  "../src/DuplicateStateDetection/TurnIndexer.h"
  "../src/LookupTable/LookupTable.h"
  "../src/LookupTable/LookupTable.cpp"
  "../src/LookupTable/CornerLookupTable.cpp"
  "../src/LookupTable/CornerLookupIndex.cpp"
  "../src/LookupTable/EdgeLookupIndex.cpp"
  "../src/LookupTable/StateIndexReverser.h"
  "../src/LookupTable/StateIndexReverser.cpp"
  "../src/Solver/Solver.h"
  "../src/Solver/Solver.cpp"
  "../src/Solver/PR_IterativeDeepeningAStar.cpp"
  "../src/Solver/SR_IterativeDeepeningAStar.cpp"
  "../src/Utils/robin-map/robin_growth_policy.h"
  "../src/Utils/robin-map/robin_hash.h"
  "../src/Utils/robin-map/robin_map.h"
  "../src/Utils/robin-map/robin_set.h"
  "../src/Utils/Constants.h"
  "../src/Utils/FileManagement.h"
  "../src/Utils/FileManagement.cpp"
  "../src/Utils/PermutationIndexer.h"
  "../src/Utils/PermutationIndexer.cpp"
  "../src/Utils/Stopwatch.h"
  "../src/Utils/Stopwatch.cpp"
  "../src/Utils/StringUtils.h"
  "../src/Utils/StringUtils.cpp"
  "../src/Utils/Utils.h"
  )

# Build the RubiksCube executable.
add_executable(Solver ${SRC})