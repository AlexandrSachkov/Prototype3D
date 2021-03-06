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

            //close window controls
            addWindowCloseEvCbk([&]() {
                stop();
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                    stop();
                }    
            });

            //scene debugging controls
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_HOME && action == GLFW_PRESS && _scene) {
                    if (_scene->getAllModels().size() == 0) {
                        return;
                    }

                    _currentModel = 0;
                    enableModel(_currentModel);
                }
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_END && action == GLFW_PRESS && _scene) {
                    ModelDesc modelDesc;
                    for (const HModel& hModel : _scene->getAllModels()) {
                        modelDesc = *_scene->getDesc(hModel);
                        modelDesc.draw = true;
                        _scene->update(hModel, modelDesc);
                    }
                }
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS && _scene) {
                    unsigned int numModels = (unsigned int)_scene->getAllModels().size();
                    if (numModels == 0) {
                        return;
                    }

                    _currentModel = (_currentModel + 1) % numModels;
                    enableModel(_currentModel);
                }
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS && _scene) {
                    unsigned int numModels = (unsigned int)_scene->getAllModels().size();
                    if (numModels == 0) {
                        return;
                    }

                    _currentModel = _currentModel > 0 ? _currentModel - 1 : numModels - 1;
                    enableModel(_currentModel);
                }
            });
            return true;
        }

        void DefaultSampleRunner::enableModel(unsigned int modelNum) {
            unsigned int numModels = (unsigned int)_scene->getAllModels().size();
            if (numModels == 0 && modelNum < numModels) {
                return;
            }

            ModelDesc modelDesc;
            for (const HModel& hModel : _scene->getAllModels()) {
                modelDesc = *_scene->getDesc(hModel);
                modelDesc.draw = false;
                _scene->update(hModel, modelDesc);
            }

            HModel hCurrModel = _scene->getAllModels()[modelNum];
            modelDesc = *_scene->getDesc(hCurrModel);
            modelDesc.draw = true;
            _scene->update(hCurrModel, modelDesc);
            P3D_ERROR_PRINT("Drawing model: " + std::to_string(modelNum));
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

        void DefaultSampleRunner::enableDebugging(SceneI* scene) {
            _scene = scene;
        }
    }
}