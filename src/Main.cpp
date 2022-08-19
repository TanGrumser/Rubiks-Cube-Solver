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

int GetPermutationIndex(array<unsigned, 8> perm) {
    int result = 0;

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7, 
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1
    };

    for (int i = 0; i < 8 - 1; i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 8; j++) {
            if (perm[j] == i) {
                result += position * coefficients[i];
                result += position * cornerPermutationCoefficients[i];
                break;
            }

            if (i < perm[j]) {
                position++;
            }
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    /*
    PermutationIndexer<8> indexex;

    array<unsigned, 8> perm = {4,1,2,0,7,5,6,3};

    std::cout << "old method yields: " << GetPermutationIndex(perm) << endl;
    std::cout << "new method yields: " << indexex.rank(perm) << endl;

    */
    CommandLineHandler::start(argc, argv);

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