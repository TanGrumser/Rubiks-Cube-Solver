#pragma once

#include "../Model/RubiksCubeState.h"

class Symmetry {

    public:
        void ApplyTo(RubiksCubeState& state);
}