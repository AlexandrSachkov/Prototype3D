#pragma once

#include "Constants.h"
#include "ResourceHandles.h"
#include "common/AABB.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include <vector>
#include <string>

namespace p3d {
    struct BoundingVolume {
        P3D_BOUNDING_VOLUME_TYPE type = P3D_BOUNDING_VOLUME_TYPE::P3D_BOUNDING_VOLUME_NONE;
        union Volume {
            Volume() {}

            AABB aabb;
        } volume;
    };

    struct TextureDesc {
        struct SurfaceDesc {
            unsigned int surfaceDim[3];     // width/height/depth
            unsigned int rowSizeBytes;      // size of a single row
            unsigned int slice2DSizeBytes;  // size of a 2d surface 
            unsigned int surfaceSizeBytes;  // full mipmap size (including depth)
        };

        std::vector<std::vector<SurfaceDesc>> surfaceMatrix; // Texture[MipMap[]]
        std::shared_ptr<char> data = nullptr;
        unsigned int dataSize;
        Format format;
        P3D_USAGE usageFlag;
        std::vector<P3D_BIND_FLAG> bindFlags;

        std::string path; //used only as reference
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

    /*struct BufferDesc {
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
    };*/

    struct ModelDesc {
        HMesh mesh;
        HMaterial material;
        glm::mat4x4 transform;
        BoundingVolume boundingVolume;
        bool drawBoundingVolume = false;
        bool draw = true;
    };

    struct MeshDesc {
        std::shared_ptr<glm::vec3> vertices = nullptr;
        unsigned int verticesSize = 0;

        std::shared_ptr<unsigned int> indices = nullptr;
        unsigned int indicesSize = 0;
        P3D_PRIMITIVE_TOPOLOGY topology = P3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

        std::shared_ptr<glm::vec2> texCoords = nullptr;
        std::shared_ptr<glm::vec3> normals = nullptr;
        std::shared_ptr<glm::vec3> tangents = nullptr;
        std::shared_ptr<glm::vec3> bitangents = nullptr;
    };

    struct MaterialDesc {
        glm::vec3 diffuseColor = {0.0f,0.0f,0.0f};
        glm::vec3 specularColor = { 0.0f,0.0f,0.0f };
        glm::vec3 transparencyColor = { 0.0f,0.0f,0.0f };
        glm::vec3 reflectionColor = { 0.0f,0.0f,0.0f };
        glm::vec3 emissionColor = { 0.0f,0.0f,0.0f };

        HTexture2dArr diffuseTex;
        HTexture2dArr normalTex;
        HTexture2dArr specularTex;
        HTexture2dArr shininessTex;
        HTexture2dArr opacityTex;
        HTexture2dArr lightmapTex;
        HTexture2dArr reflectionTex;
        HTexture2dArr emissionTex;

        float shininess = 0.0f;
        float shininessStrength = 1.0f;
        float opacity = 1.0f;
        float reflectivity = 0.0f;
        float refracti = 1.0f;
        
        P3D_TEX_MAP_MODE texMapMode = P3D_TEX_MAP_WRAP;
        bool wireframe = false;

        //P3D_BLEND_MODE blendMode; since only 1 texture of each type is allowed, we might not need it

        bool isTransparent() const {
            return opacity < 1.0f || opacityTex.isValid();
        }
    };

    struct SceneProperties {
        glm::vec3 ambientLight = { 1.0f,1.0f,1.0f };
        bool drawBoundingVolumes = false;
    };

    struct LightDesc {
        struct PointLight {
            glm::vec3 position;
            glm::vec3 ambientColor;
            glm::vec3 diffuseColor;
            glm::vec3 specularColor;

            float range = 0.0f;
            float constAttenuation = 0.0f;
            float linearAttenuation = 0.0f;
            float quadraticAttenuation = 0.0f;
        };

        P3D_LIGHT_TYPE type;
        union Light {
            Light() {}

            PointLight point;
        } desc;
    };
}