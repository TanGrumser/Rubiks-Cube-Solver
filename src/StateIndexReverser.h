#pragma once

#include <iostream>
#include "RubicsCubeState.h"
#include "StateIndex.h"
#include "Definitions.h"
#include "Constants.h"

namespace StateIndexReverser {
    RubicsCubeState GetStateFromIndex(StateIndex index);
    void Test();
}