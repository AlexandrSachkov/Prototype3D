#pragma once

#include "glm/mat4x4.hpp"

namespace p3d {
    namespace dx {
        __declspec(align(16))
        struct TransformData 
        {
            //glm::mat4x4 world;
            //glm::mat4x4 worldInvTrans;
            glm::mat4x4 wvp;
        };
    }
}