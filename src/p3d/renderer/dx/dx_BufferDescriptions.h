#pragma once

#include "glm/mat4x4.hpp"

namespace p3d {
    namespace dx {
        __declspec(align(16))
        struct TransformData 
        {
            glm::mat4x4 world;
            glm::mat4x4 worldInvTrans;
            glm::mat4x4 wvp;
        };

        __declspec(align(16))
            struct MaterialData {
            glm::vec3 diffuseColor = { 0.0f,0.0f,0.0f };
            float shininess = 0.0f;
            glm::vec3 specularColor = { 0.0f,0.0f,0.0f };
            float shininessStrength = 1.0f;
            glm::vec3 transparencyColor = { 0.0f,0.0f,0.0f };
            float reflectivity = 0.0f;
            glm::vec3 reflectionColor = { 0.0f,0.0f,0.0f };
            float refracti = 1.0f;
            glm::vec3 emissionColor = { 0.0f,0.0f,0.0f };
            float opacity = 1.0f;

            int hasDiffuteTex = 0;
            int hasEmissionTex = 0;
            int hasLightmapTex = 0;
            int hasNormalTex = 0;
            int hasOpacityTex = 0;
            int hasReflectionTex = 0;
            int hasShininessTex = 0;
            int hasSpecularTex = 0;
        };

        __declspec(align(16))
            struct SceneConstants {
            glm::vec3 ambientLight = { 0.0f,0.0f,0.0f };
        };
    }
}