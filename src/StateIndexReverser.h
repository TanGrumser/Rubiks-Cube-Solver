#pragma once

#include <iostream>
#include "RubicsCubeState.h"
#include "StateIndex.h"
#include "Definitions.h"
#include "Constants.h"
#include "PermutationIndexer.h"
#include "Stopwatch.h"

namespace StateIndexReverser {
    static PermutationIndexer<8> cornerIndexer;
    static PermutationIndexer<12> egdeIndexer;

    RubicsCubeState GetStateFromIndex(StateIndex index);
    void Test();
}