#pragma once

#include <iostream>
#include "../Model/RubicsCubeState.h"
#include "../Model/StateIndex.h"
#include "../Utils/Constants.h"
#include "../Utils/PermutationIndexer.h"
#include "../Utils/Stopwatch.h"

namespace StateIndexReverser {
    static PermutationIndexer<8> cornerIndexer;
    static PermutationIndexer<12> egdeIndexer;

    RubicsCubeState GetStateFromIndex(StateIndex index);
    void Test();
}