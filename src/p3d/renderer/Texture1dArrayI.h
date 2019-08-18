#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
    class Texture1dArrayI {
    public:
        virtual ~Texture1dArrayI() {};
        virtual const TextureDesc& getDescription() const = 0;
    };
}
