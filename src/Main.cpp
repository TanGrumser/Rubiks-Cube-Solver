#include "CommandLineHandler.h"

//XXX
#include "LookupTable/StateIndexReverser.h"
#include "Utils/PermutationIndexer.h"
#include <array>
#include <vector>
#include <algorithm>
#include "LookupTable/LookupTable.h"
#include "Utils/FileManagement.h"

void checkShuffleSolvePair(std::string path);


int main(int argc, char *argv[]) {

    //CommandLineHandler::start(argc, argv);
    checkShuffleSolvePair("dsResults.txt");
    return 0;
}

void checkShuffleSolvePair(std::string path) {
 std::vector<string> lines = FileManagement::parseALlLines(path);
    int solved = 0;
    int unsolved = 0;
    int total = 0;


    std::cout << "Unsolved at " << lines.size() << endl;

    for (int i = 0; i < lines.size(); i += 2) {
        std::vector<Turn> shuffle = Turn::parseShuffle(lines[i]);
        std::vector<Turn> solution = Turn::parseShuffle(lines[i + 1]);

        RubiksCubeState cube = RubiksCubeState::InitialState().Copy();

        for (Turn turn : shuffle) {
            cube.ApplyTurn(turn);
        }

        for (Turn turn : solution) {
            cube.ApplyTurn(turn);
        }
        
        total++;

        if (!cube.Equals(RubiksCubeState::InitialState())) {
            unsolved++;
        } else {
            solved++;
        }
    }


    std::cout << "Solved count:  " << solved << ", unsolved count: " << unsolved << ", total: " << total << endl;
}