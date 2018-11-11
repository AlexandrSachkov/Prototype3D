#pragma once
#include "../ResourceDescriptions.h"

namespace p3d {
    class RasterizerI {
    public:
        virtual ~RasterizerI() {};
        virtual const RasterizerDesc& getDescription() const = 0;
    };
}