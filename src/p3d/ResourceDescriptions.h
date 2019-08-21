#pragma once

#include "Constants.h"
#include "ResourceHandles.h"

#include <vector>
#include <string>

namespace p3d {
    struct TextureDesc {
        struct SurfaceDesc {
            unsigned int surfaceDim[3];     // width/height/depth
            unsigned int rowSizeBytes;      // size of a single row
            unsigned int slice2DSizeBytes;  // size of a 2d surface 
            unsigned int surfaceSizeBytes;  // full mipmap size (including depth)
        };

        std::vector<std::vector<SurfaceDesc>> surfaceMatrix; // Texture[MipMap[]]
        char* data;
        unsigned int dataSize;
        Format format;
        P3D_USAGE usageFlag;
        std::vector<P3D_BIND_FLAG> bindFlags;
        bool generateMipMaps;
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

    struct ModelDesc {
        HMesh mesh;
        HMaterial material;
    };

    struct MeshDesc {

    };

    struct MaterialDesc {

    };
}