#pragma once

#include "common/p3d_Uuid.h"

namespace p3d {
    enum class SCENE_OBJECT_TYPE {
        MODEL,
        LIGHT,

        // size of enum
        SIZE
    };

    struct HResource {
        HResource() : buffPosition(0), uuid(0) {}
        HResource(unsigned int buffPosition, UUID uuid)
            : buffPosition(buffPosition), uuid(uuid) {}

        bool isValid() {
            return uuid != 0;
        }

        const unsigned int buffPosition;
        const UUID uuid;
    };

    // Scene objects
    struct HSceneObject : public HResource {
        HSceneObject(SCENE_OBJECT_TYPE type) : type(type) {}
        HSceneObject(SCENE_OBJECT_TYPE type, unsigned int buffPosition, UUID uuid)
            : type(type), HResource(buffPosition, uuid) {}

        const SCENE_OBJECT_TYPE type;
    };

    struct HModel : public HSceneObject {
        HModel() : HSceneObject(SCENE_OBJECT_TYPE::MODEL) {}
        HModel(unsigned int buffPosition, UUID uuid)
            : HSceneObject(SCENE_OBJECT_TYPE::MODEL, buffPosition, uuid) {}
    };
    struct HLight : public HSceneObject {
        HLight() : HSceneObject(SCENE_OBJECT_TYPE::LIGHT) {}
        HLight(unsigned int buffPosition, UUID uuid)
            : HSceneObject(SCENE_OBJECT_TYPE::LIGHT, buffPosition, uuid) {
        }
    };

    // Resources
    struct HMesh : public HResource {
        HMesh() : HResource() {}
        HMesh(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HMaterial : public HResource {
        HMaterial() : HResource() {}
        HMaterial(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HTexture1dArr : public HResource {
        HTexture1dArr() : HResource() {}
        HTexture1dArr(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HTexture2dArr : public HResource {
        HTexture2dArr() : HResource() {}
        HTexture2dArr(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HTexture3d : public HResource {
        HTexture3d() : HResource() {}
        HTexture3d(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HVertexShader : public HResource {
        HVertexShader() : HResource() {}
        HVertexShader(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };

    struct HPixelShader : public HResource {
        HPixelShader() : HResource() {}
        HPixelShader(unsigned int buffPosition, UUID uuid) : HResource(buffPosition, uuid) {}
    };
}