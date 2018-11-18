#pragma once

#include "Constants.h"

#include <vector>
#include <string>

namespace p3d {
    struct Texture1dArrayDesc {

    };

    struct Texture2dArrayDesc {
        struct SurfaceDesc {
            unsigned int surfaceDim[2];
            void* data;
            unsigned int dataSizeBytes;
            unsigned int rowSizeBytes;
        };

        std::vector<std::vector<SurfaceDesc>> surfaceMatrix; // Texture[MipMap[]]
        Format format;
        P3D_USAGE usageFlag;
        std::vector<P3D_BIND_FLAG> bindFlags;
        bool generateMipMaps;
    };

    struct Texture3dDesc {
        
    };

    struct VertexShaderDesc {
        struct InputElementDesc {
            std::string elementName;
            P3D_VECTOR_FORMAT dataFormat;
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
        void* data = nullptr;
        P3D_USAGE usageFlag;
        P3D_VECTOR_FORMAT dataFormat;
        unsigned int length;
    };

    struct RasterizerDesc {
        P3D_FILL_MODE fillMode;
        P3D_CULL_MODE cullMode;
        bool frontCounterClockwise;
    };
}