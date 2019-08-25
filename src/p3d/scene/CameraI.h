#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace p3d {
    class CameraI {
    public:
        virtual ~CameraI() {}

        virtual glm::mat4x4 getView() const = 0;
        virtual glm::vec3 getEye() const = 0;
    };
}