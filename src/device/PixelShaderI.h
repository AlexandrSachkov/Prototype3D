#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
    class PixelShaderI {
    public:
        virtual ~PixelShaderI() {}
        virtual const PixelShaderDesc& getDescription() const = 0;
    };
}
