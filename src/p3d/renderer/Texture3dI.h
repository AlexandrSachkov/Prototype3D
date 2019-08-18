#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
    class Texture3dI {
    public:
        virtual ~Texture3dI() {};
        virtual const TextureDesc& getDescription() const = 0;
    };
}