#pragma once

#include "Constants.h"

#include <vector>
#include <string>

namespace p3d {
    struct Texture1dArrayDesc {

    };

    struct Texture2dArrayDesc {

    };

    struct Texture3dDesc {

    };

    struct VertexShaderDesc {
        struct InputElementDesc {
            std::string elementName;
            P3D_FORMAT dataFormat;
            unsigned int dataSizeBytes;
            unsigned int inputSlot;
        };

        std::vector<InputElementDesc> inputDesc;
        std::string hlslSource;
        std::string shaderEntryPoint;
    };

    struct PixelShaderDesc {
        std::string hlslSource;
        std::string shaderEntryPoint;
    };

    struct BufferDesc {
        std::vector<P3D_BIND_FLAG> bindFlags;
        P3D_USAGE usageFlag;
        void* data = nullptr;
        unsigned int strideBytes; // size of each element
        unsigned int length;      // number of elements
    };

    struct RasterizerDesc {
        P3D_FILL_MODE fillMode;
        P3D_CULL_MODE cullMode;
        bool frontCounterClockwise;
    };
}