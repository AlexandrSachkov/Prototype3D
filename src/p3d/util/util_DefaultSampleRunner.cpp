#include "util_DefaultSampleRunner.h"
#include "../assert.h"

namespace p3d {
    namespace util {
        DefaultSampleRunner::DefaultSampleRunner()
            : _running(false), _runProcedure([]() {}) {}

        DefaultSampleRunner::~DefaultSampleRunner() {}

        bool DefaultSampleRunner::initialize(
            const std::string title,
            const unsigned int screenDim[2],
            bool fullscreen,
            bool decorated,
            bool resizable,
            bool enableCursor
        ) {
            P3D_ASSERT_R(GlfwWindowManager::initialize(
                title,
                screenDim,
                fullscreen,
                decorated,
                resizable,
                enableCursor
            ), "Failed to initialize window manager");

            addWindowCloseEvCbk([&]() {
                stop();
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                    stop();
                }    
            });
            return true;
        }

        void DefaultSampleRunner::setRunProcedure(std::function<void()> procedure) {
            _runProcedure = procedure;
        }

        void DefaultSampleRunner::setCameraController(CameraControllerI* cameraController, float moveStepSize, float rotationMultiplier) {
            _cameraController = cameraController;
            _camMoveStepSize = moveStepSize;
            _camRotationMultiplier = rotationMultiplier;
        }

        void DefaultSampleRunner::start() {
            _running = true;
            do {
                pollEvents();
                updateCameraController();
                _runProcedure();
            } while (_running);
        }

        void DefaultSampleRunner::stop() {
            _running = false;
        }

        void DefaultSampleRunner::updateCameraController() {
            if (!_cameraController) {
                return;
            }

            int wState = getKeyState(GLFW_KEY_W);
            if (wState == GLFW_PRESS || wState == GLFW_REPEAT) {
                _cameraController->move(_camMoveStepSize, 0.0f);
            }

            int sState = getKeyState(GLFW_KEY_S);
            if (sState == GLFW_PRESS || sState == GLFW_REPEAT) {
                _cameraController->move(-_camMoveStepSize, 0.0f);
            }
            
            int aState = getKeyState(GLFW_KEY_A);
            if (aState == GLFW_PRESS || aState == GLFW_REPEAT) {
                _cameraController->move(0.0f, -_camMoveStepSize);
            }
            
            int dState = getKeyState(GLFW_KEY_D);
            if (dState == GLFW_PRESS || dState == GLFW_REPEAT) {
                _cameraController->move(0.0f, _camMoveStepSize);
            }

            double cursorX, cursorY;
            getCursorPosition(cursorX, cursorY);

            if (_firstCameraUpdate) {
                _lastCursorX = cursorX;
                _lastCursorY = cursorY;
                _firstCameraUpdate = false;
            }

            double yaw = cursorX - _lastCursorX;
            double pitch = cursorY - _lastCursorY;
            _lastCursorX = cursorX;
            _lastCursorY = cursorY;

            _cameraController->rotate((float)(pitch * _camRotationMultiplier), (float)(yaw * _camRotationMultiplier));
        }
    }
}