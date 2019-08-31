#pragma once

#include "util_GlfwWindowManager.h"
#include "util_CameraControllerI.h"
#include <functional>

namespace p3d {
    namespace util {
        class DefaultSampleRunner : public GlfwWindowManager {
        public:
            DefaultSampleRunner();
            ~DefaultSampleRunner();

            bool initialize(
                const std::string title,
                const unsigned int screenDim[2],
                bool fullscreen,
                bool decorated,
                bool resizable,
                bool enableCursor
            );

            void setRunProcedure(std::function<void()> procedure);
            void setCameraController(CameraControllerI* cameraController, float moveStepSize, float rotationMultiplier);
            void start();
            void stop();

        private:
            void updateCameraController();
            bool _running;
            std::function<void()> _runProcedure;

            CameraControllerI* _cameraController = nullptr;
            float _camMoveStepSize = 0.0f;
            float _camRotationMultiplier = 0.0f;

            double _lastCursorX = 0;
            double _lastCursorY = 0;
            bool _firstCameraUpdate = true;
        };
    }
}
