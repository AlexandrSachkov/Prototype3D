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
            glm::vec3 ambientColor = { 0.0f,0.0f,0.0f };
            float opacity = 1.0f;
            glm::vec3 diffuseColor = { 0.0f,0.0f,0.0f };
            float shininess = 0.0f;
            glm::vec3 specularColor = { 0.0f,0.0f,0.0f };
            float shininessStrength = 1.0f;
            glm::vec3 transparencyColor = { 0.0f,0.0f,0.0f };
            float reflectivity = 0.0f;
            glm::vec3 reflectionColor = { 0.0f,0.0f,0.0f };
            float refracti = 1.0f;
            glm::vec3 emissionColor = { 0.0f,0.0f,0.0f };

            bool hasAmbientTex = false;
            bool hasDiffuteTex = false;
            bool hasEmissionTex = false;
            bool hasLightmapTex = false;
            bool hasNormalTex = false;
            bool hasOpacityTex = false;
            bool hasReflectionTex = false;
            bool hasShininessTex = false;
            bool hasSpecularTex = false;
        };
    }
}