cmake_minimum_required(VERSION 2.4)
project(RubiksCubeSolver)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-pthread")

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

set(HEADERS
  "src/CommandLineHandler.h"
  "src/Model/RubiksCubePiece.h"
  "src/Model/RubiksCubeState.h"
  "src/Model/RubiksCubeStateShift.h"
  "src/Model/StateIndex.h"
  "src/Model/StateIndexHasher.h"
  "src/Model/Turn.h"
  "src/DuplicateStateDetection/DuplicateState.cpp"
  "src/DuplicateStateDetection/DuplicateState.h"
  "src/DuplicateStateDetection/TurnIndexer.h"
  "src/LookupTable/LookupTable.h"
  "src/LookupTable/StateIndexReverser.h"
  "src/LookupTable/LookupTableGenerator.h"
  "src/Solver/Solver.h"
  "src/Utils/robin-map/robin_growth_policy.h"
  "src/Utils/robin-map/robin_hash.h"
  "src/Utils/robin-map/robin_map.h"
  "src/Utils/robin-map/robin_set.h"
  "src/Utils/Constants.h"
  "src/Utils/FileManagement.h"
  "src/Utils/PermutationIndexer.h"
  "src/Utils/Stopwatch.h"
  "src/Utils/StringUtils.h"
  "src/Utils/Utils.h"
  "src/Utils/Logging/Logger.h"
  "src/Utils/Logging/FileLogger.h"
  "src/Utils/Logging/ConsoleLogger.h"
  )

# All source files.
set(SRC
  "src/Main.cpp"
  "src/CommandLineHandler.cpp"
  "src/Model/RubiksCubeRotator.cpp"
  "src/Model/RubiksCubeState.cpp"
  "src/Model/Turn.cpp"
  "src/DuplicateStateDetection/DuplicateStateTurnIndex.cpp"
  "src/DuplicateStateDetection/TurnIndexer.cpp"
  "src/LookupTable/LookupTable.cpp"
  "src/LookupTable/CornerLookupTable.cpp"
  "src/LookupTable/StateIndexReverser.cpp"
  "src/LookupTable/E1LookupTable.cpp"
  "src/LookupTable/E2LookupTable.cpp"
  "src/LookupTable/EdgeLookupTable.cpp"
  "src/LookupTable/EdgePermutationLookupTable.cpp"
  "src/LookupTable/LookupTableGenerator.cpp"
  "src/Solver/Solver.cpp"
  "src/Solver/PR_IterativeDeepeningAStar.cpp"
  "src/Solver/SR_IterativeDeepeningAStar.cpp"
  "src/Utils/FileManagement.cpp"
  "src/Utils/PermutationIndexer.cpp"
  "src/Utils/Stopwatch.cpp"
  "src/Utils/StringUtils.cpp"
  "src/Utils/Logging/ConsoleLogger.cpp"
  "src/Utils/Logging/FileLogger.cpp"
  )

# Build the RubiksCube executable.
add_executable(Solver ${SRC} ${HEADERS})

#TODOS
#