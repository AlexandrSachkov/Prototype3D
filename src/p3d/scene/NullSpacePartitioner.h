#pragma once

#include "SpacePartitionerI.h"

namespace p3d {
    class NullSpacePartitioner : public SpacePartitionerI {
    public:
        NullSpacePartitioner() {}
    };
}