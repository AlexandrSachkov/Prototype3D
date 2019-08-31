#pragma once

namespace p3d {
    namespace util {
        class CameraControllerI {
        public:
            virtual ~CameraControllerI() {}
            virtual void move(float fwd, float right) = 0;
            virtual void rotate(float pitchRad, float yawRad) = 0;
        };
    }
}