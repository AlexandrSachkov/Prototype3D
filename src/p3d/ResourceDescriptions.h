#pragma once

#include "Constants.h"
#include "ResourceHandles.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

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
        glm::vec3* positions = nullptr;
        unsigned int positionsSize = 0;

        glm::vec3* indexes = nullptr;
        unsigned int indexesSize = 0;
        P3D_PRIMITIVE_TOPOLOGY topology = P3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

        glm::vec4* colors = nullptr;
        unsigned int colorsSize = 0;

        glm::vec3* texCoords = nullptr;
        unsigned int texCoordsSize = 0;

        glm::vec3* normals = nullptr;
        unsigned int normalsSize = 0;

        glm::vec3* tangents = nullptr;
        unsigned int tangentsSize = 0;
    };

    struct MaterialDesc {
        glm::vec3 diffuseColor = {0.0f,0.0f,0.0f};
        HTexture2dArr diffuseTex;
        TEX_MAP_MODE diffuseMapMode = TEX_MAP_CLAMP;

        glm::vec3 normalColor = { 0.0f,0.0f,0.0f };
        HTexture2dArr normalTex;
        TEX_MAP_MODE normalMapMode = TEX_MAP_CLAMP;

        glm::vec3 transparencyColor = { 0.0f,0.0f,0.0f };
        HTexture2dArr transparencyTex;
        TEX_MAP_MODE transparencyMapMode = TEX_MAP_CLAMP;

        glm::vec3 ambientColor = { 0.0f,0.0f,0.0f };
        glm::vec3 specularColor = { 0.0f,0.0f,0.0f };

        float shininess = 0.0f;
        float shininessStrength = 0.0f;
        float opacity = 1.0f;
        bool wireframe = false;

        //P3D_BLEND_MODE blendMode; since only 1 texture of each type is allowed, we might not need it
    };
}