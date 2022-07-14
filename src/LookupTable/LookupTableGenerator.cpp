#include <thread>

#include "LookupTable.h"
#include "LookupTableGenerator.h"
#include "../Utils/Stopwatch.h"
#include "../Utils/FileManagement.h"
#include "../Utils/utils.h"

void LookupTableGenerator::WriteLookupTableToFile(string path) {
    FileManagement::WriteBufferToFile(path, lookupTable.data(), lookupTableSize);
}

LookupTableGenerator::LookupTableGenerator(uint64_t lookupTableSize, IndexCalculator indexCalculator, StateCalculator stateCalculator, Logger* logger) :
    indexCalculator(indexCalculator),
    stateCalculator(stateCalculator),
    lookupTableSize(lookupTableSize),
    lookupTable(lookupTableSize, UNINITIALIZED),
    logger(logger),
    currentDepth(0),
    reachedStates(0),
    currentDepthStates(0)
    {}

// ------------------ Helper Methods ---------------------------

inline bool LookupTableGenerator::hasGenerationFinished() {
    return reachedStates == lookupTableSize;
}

void inline LookupTableGenerator::ClenaupReachedFlags() {
    for (uint64_t i = 0; i < lookupTable.capacity(); i++) {
        if (lookupTable[i] != UNINITIALIZED) {
            lookupTable[i] &= 0b01111111;
        }
    }
}

void inline LookupTableGenerator::SetReachedFlag(uint64_t index) {
    lookupTable[index] = lookupTable[index] | 0b10000000;
}

bool inline LookupTableGenerator::GetReachedFlag(uint64_t index) {
    return lookupTable[index] != UNINITIALIZED && (lookupTable[index] & 0b10000000) != 0;
}

void LookupTableGenerator::StartLogger(long long loggingInterval) {
    std::thread([this, loggingInterval] {
        GenerationLogger(loggingInterval);
    }).detach();
}

void LookupTableGenerator::GenerationLogger(long long loggingInterval) {
    StopWatch timer;
    
    timer.StartTimer();


    while (!hasGenerationFinished()) {
        double totalProgress = (double)reachedStates / (double)lookupTableSize * 100.0;
        double depthProgress = (double)processedStates / (double)lookupTableSize * 100.0;
        
        consoleMutex.lock();
        switch (generationMode) {
            case GenerationMode::IterativeDeepeningDepthFirstSearchMode:
                logger->updateLastLine(
                    "Elapsed Time: " + timer.GetFormattedTimeInSeconds() +
                    ", reachedStates: " + to_string(reachedStates) +
                    ", progress: " + Utils::convert(totalProgress, 4) + "%" +
                    ", depth: " + to_string(currentDepth) +
                    ", states at depth: " + to_string(currentDepthStates)
                );
            break;

            case GenerationMode::InverseStateIndexSearchMode:
                logger->updateLastLine(
                    "Elapsed Time: " + timer.GetFormattedTimeInSeconds() +
                    ", reachedStates: " + to_string(reachedStates) +
                    ", depth progress: " + Utils::convert(depthProgress, 4) + "%" +
                    ", total progress: " + Utils::convert(totalProgress, 4) + "%" +
                    ", depth: " + to_string(currentDepth)
                );
            break;
        }        
            
        consoleMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(loggingInterval));
    }

    logger->logNewLine("Toal time required: " + timer.GetFormattedTimeInSeconds());
}

// ----------- kickoff methods ---------------------------

void LookupTableGenerator::PopulateWithIterativeDeepeningDepthFirstSearch(char maxDepth, int threadCount) {
    std::vector<std::thread> threads(threadCount);
    generationMode = GenerationMode::IterativeDeepeningDepthFirstSearchMode;
    StopWatch timer;

    while (!hasGenerationFinished() && currentDepth <= maxDepth) {
        currentDepthStates = 0;
        timer.StartTimer();

        for (int i = 0; i < threadCount; i++) {
            std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, threadCount);
            
            std::thread thread([this, exploredTurns] {
                IterativeDeepeningDepthFirstSearch(RubiksCubeState::InitialState().Copy(), 0, this->currentDepth, Turn::Empty(), exploredTurns);
            });
            
            threads[i] = std::move(thread);
        }

        for (int i = 0; i < threadCount; i++) {
            threads[i].join();
        }
        
        consoleMutex.lock();
        logger->logNewLine("Reached states at depth " + to_string(currentDepth) + " are " + to_string(currentDepthStates) + " step took " + timer.GetFormattedTimeInSeconds());
        consoleMutex.unlock();

        // Reset all reached state flags.
        ClenaupReachedFlags();

        currentDepth++;
    }
}

void LookupTableGenerator::PopulateWithInverseStateIndexSearch(int threadCount) {
    std::vector<std::thread> threads(threadCount);
    generationMode = GenerationMode::InverseStateIndexSearchMode;
    StopWatch timer;

    while (!hasGenerationFinished()) {
        timer.StartTimer();
        processedStates = 0;
        currentDepthStates = 0;

        for (int i = 0; i < threadCount; i++) {
            uint64_t startIndex = lookupTableSize / threadCount * i;
            uint64_t endIndex = 
                i == threadCount - 1 ? 
                lookupTableSize : 
                lookupTableSize / threadCount * (i + 1);
            
            std::thread thread([this, startIndex, endIndex] {
                InverseStateIndexSearch(currentDepth, startIndex, endIndex);
            });

            threads[i] = std::move(thread);
        }

        for (int i = 0; i < threadCount; i++) {
            threads[i].join();
        }
        
        consoleMutex.lock();
        logger->logNewLine("Reached states at depth " + to_string(currentDepth) + " are " + to_string(currentDepthStates) + " step took " + timer.GetFormattedTimeInSeconds());
        consoleMutex.unlock();

        currentDepth++;
    }
}

// ---------------------- Main work horse methods ----------------------------

void LookupTableGenerator::IterativeDeepeningDepthFirstSearch(RubiksCubeState& state, char depth, char maxDepth, Turn lastTurn, vector<Turn> turnsToExplore) {
    if (hasGenerationFinished()) {
        return;
    }

    uint64_t lookupIndex = indexCalculator(state);
    
    if (lookupIndex != 0 && GetReachedFlag(lookupIndex)) {
        return;
    }

    char& entry = lookupTable[lookupIndex];
    
    lookupTableMutex.lock();
    
    if (entry == UNINITIALIZED) {
		// This state has never been reached before.
		entry = depth;
		reachedStates++;
        currentDepthStates++;
	}

    lookupTableMutex.unlock();
      
    char entryDepth = entry & 0b01111111;

    if (depth > entryDepth) {
        // we reached an explored state, with a longer path and don't want to continue, since there are no follwoing states will habve shorter paths.
		return;
	}
    
    SetReachedFlag(lookupIndex);

    if (depth < maxDepth) {
        for (Turn turn : turnsToExplore) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }

            state.ApplyTurn(turn);
            IterativeDeepeningDepthFirstSearch(state, depth + 1, maxDepth, turn, Turn::AllTurns);
            state.ApplyTurn(turn.Inverse());
        }
    }
}

void LookupTableGenerator::InverseStateIndexSearch(char depth, uint64_t startIndex, uint64_t endIndex) {
    // We go through all entries in the lookup table
    for (uint64_t index = startIndex; index < endIndex; index++) {
        processedStates++;

        // When a value has not been initialized yet.
        if (lookupTable[index] == UNINITIALIZED) {
            // get the corrosponding state
            RubiksCubeState state = stateCalculator(index);

            // and look at all neighbour states
            for (Turn turn : Turn::AllTurns) {
                state.ApplyTurn(turn);
                
                uint64_t neighbourIndex = indexCalculator(state);
                
                // when a neighbour state has depth - 1 distance, the unitilized state can be reached in depth moves. 
                if (lookupTable[neighbourIndex] == depth - 1) {
                    lookupTable[index] = depth;
                    reachedStates++;
                    currentDepthStates++;
                    break;
                }

                state.ApplyTurn(turn.Inverse());
            }
        }
    }
}
