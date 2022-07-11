#pragma once

#include <vector>
#include <atomic>
#include <mutex>

#include "../Utils/Logging/Logger.h"
#include "../Model/RubiksCubeState.h"

class LookupTableGenerator {

    enum GenerationMode {IterativeDeepeningDepthFirstSearchMode, InverseStateIndexSearchMode};

    typedef uint64_t(*IndexCalculator)(RubiksCubeState&);
    typedef RubiksCubeState(*StateCalculator)(uint64_t);

    const char UNINITIALIZED = -1;

    private:
        vector<char> lookupTable;

        uint64_t lookupTableSize;
        IndexCalculator indexCalculator;
        StateCalculator stateCalculator;
        
        char currentDepth;
        std::atomic_int64_t reachedStates;
        std::atomic_int64_t currentDepthStates;
        std::atomic_int64_t processedStates;

        std::mutex lookupTableMutex;
        std::mutex consoleMutex;
        
        GenerationMode generationMode;
        Logger* logger;

        bool hasGenerationFinished();

        // Utility helper methods.
        void ClenaupReachedFlags();
        void SetReachedFlag(uint64_t index);
        bool GetReachedFlag(uint64_t index);

        // Work horses
        void IterativeDeepeningDepthFirstSearch(RubiksCubeState& state, char depth, char maxDepth, Turn lastTurn, vector<Turn> turnsToExplore);
        void InverseStateIndexSearch(char depth, uint64_t startIndex, uint64_t endIndex);

    public:
        LookupTableGenerator(
            uint64_t lookupTableSize,
            IndexCalculator indexCalculator, 
            StateCalculator stateCalculator, 
            Logger* logger
        );
        void GenerationLogger(long long loggingInterval);

        void StartLogger(long long loggingInterval);
        void WriteLookupTableToFile(string path);
        void PopulateWithIterativeDeepeningDepthFirstSearch(char maxDepth, int threadCount);
        void PopulateWithInverseStateIndexSearch(int threadCount);
        
};