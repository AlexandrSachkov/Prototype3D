#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
    class VertexShaderI {
    public:
        virtual ~VertexShaderI() {}
        virtual const VertexShaderDesc& getDescription() const = 0;
    };
}