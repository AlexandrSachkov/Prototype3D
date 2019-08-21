#pragma once

namespace p3d {
    enum class SCENE_OBJECT_TYPE {
        MODEL,
        LIGHT,

        // size of enum
        SIZE
    };

    struct HResource {
        HResource() : buffPosition(0), uuid(0) {}
        HResource(unsigned int buffPosition, unsigned long long uuid)
            : buffPosition(buffPosition), uuid(uuid) {}

        bool isValid() {
            return uuid != 0;
        }

        const unsigned int buffPosition;
        const unsigned long long uuid;
    };

    // Scene objects
    struct HSceneObject : public HResource {
        HSceneObject(SCENE_OBJECT_TYPE type) : type(type) {}
        HSceneObject(SCENE_OBJECT_TYPE type, unsigned int buffPosition, unsigned long long uuid) 
            : type(type), HResource(buffPosition, uuid) {}

        const SCENE_OBJECT_TYPE type;
    };

    struct HModel : public HSceneObject {
        HModel() : HSceneObject(SCENE_OBJECT_TYPE::MODEL) {}
        HModel(unsigned int buffPosition, unsigned long long uuid)
            : HSceneObject(SCENE_OBJECT_TYPE::MODEL, buffPosition, uuid) {}
    };
    struct HLight : public HSceneObject {
        HLight() : HSceneObject(SCENE_OBJECT_TYPE::LIGHT) {}
        HLight(unsigned int buffPosition, unsigned long long uuid)
            : HSceneObject(SCENE_OBJECT_TYPE::LIGHT, buffPosition, uuid) {
        }
    };

    // Resources
    struct HMesh : public HResource {};
    struct HMaterial : public HResource {};
    struct HTexture2dArr : public HResource {};
    struct HVertexShader : public HResource {};
    struct HPixelShader : public HResource {};
}