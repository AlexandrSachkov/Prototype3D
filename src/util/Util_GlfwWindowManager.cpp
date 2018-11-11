#include "util_GlfwWindowManager.h"

#include "../assert.h"
#include <assert.h>

namespace p3d {
    namespace util {
        GlfwWindowManager::GlfwWindowManager() :
            _window(nullptr) {
        }

        GlfwWindowManager::~GlfwWindowManager() {
            if (_window) {
                glfwDestroyWindow(_window);
                _window = nullptr;
            }    
            glfwTerminate();
        }


        bool GlfwWindowManager::initialize(
            unsigned int screenDim[2],
            bool fullscreen,
            bool decorated,
            bool resizable,
            bool enableCurs,
            std::string title
        ) {
            glfwSetErrorCallback(glfwErrorCallback);
            P3D_ASSERT_R(glfwInit(), "GLFW failed to initialize");

            glfwWindowHint(GLFW_RESIZABLE, resizable);
            glfwWindowHint(GLFW_DECORATED, decorated);

            if (fullscreen) {
                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                _window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
            } else {
                _window = glfwCreateWindow(screenDim[0], screenDim[1], title.c_str(), NULL, NULL);
            }

            if (!_window) {
                glfwTerminate();
                P3D_ASSERT_R(false, "GLFW Window failed to initialize");
            }
            glfwSetWindowUserPointer(_window, this);
            setWindowCallbacks(_window);
            enableCursor(enableCurs);

            return true;
        }

        void GlfwWindowManager::enableCursor(bool enable) {
            glfwSetInputMode(_window, GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }

        void GlfwWindowManager::setWindowCallbacks(GLFWwindow* window) {
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireKeyEv(key, scancode, action, mods);
            });
            glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireCharEv(codepoint);
            });
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireMouseBtnEv(button, action, mods);
            });
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireCursorPosEv(xPos, yPos);
            });
            glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireScrollEv(xOffset, yOffset);
            });
            glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireWindowSizeEv(width, height);
            });
            glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
                auto& windowMngr = *static_cast<GlfwWindowManager*>(glfwGetWindowUserPointer(window));
                windowMngr.fireWindowCloseEv();
            });
        }

        void GlfwWindowManager::getClientSize(unsigned int clientDim[2]) {
            glfwGetWindowSize(_window, (int*)&clientDim[0], (int*)&clientDim[1]);
        }

        HWND GlfwWindowManager::getWindowHandle() {
            return glfwGetWin32Window(_window);
        }

        void GlfwWindowManager::glfwErrorCallback(int error, const char* description) {
            printf("%s\n", description);
            switch (error) {
            case GLFW_NO_CURRENT_CONTEXT:
                assert(false && "GLFW_NO_CURRENT_CONTEXT");
            case GLFW_OUT_OF_MEMORY:
                assert(false && "GLFW_NO_CURRENT_CONTEXT");
            case GLFW_PLATFORM_ERROR:
                assert(false && "GLFW_NO_CURRENT_CONTEXT");
            case GLFW_VERSION_UNAVAILABLE:
                assert(false && "GLFW_NO_CURRENT_CONTEXT");
            case GLFW_FORMAT_UNAVAILABLE:
                assert(false && "GLFW_NO_CURRENT_CONTEXT");
            default:
                assert(false && "GLFW_UNKNOWN_ERROR");
            }
        }

        void GlfwWindowManager::pollEvents() {
            glfwPollEvents();
        }

        void GlfwWindowManager::addKeyEvCbk(std::function<void(int key, int scancode, int action, int mods)> callback) {
            _keyEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addCharEvCbk(std::function<void(unsigned int codepoint)> callback) {
            _charEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addMouseBtnEvCbk(std::function<void(int button, int action, int mods)> callback) {
            _mouseBtnEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addCursorPosEvCbk(std::function<void(double xPos, double yPos)> callback) {
            _cursorPosEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addScrollEvCbk(std::function<void(double xOffset, double yOffset)> callback) {
            _scrollEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addWindowSizeEvCbk(std::function<void(int width, int height)> callback) {
            _windowSizeEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::addWindowCloseEvCbk(std::function<void()> callback) {
            _windowCloseEvCallbacks.push_back(callback);
        }

        void GlfwWindowManager::fireKeyEv(int key, int scancode, int action, int mods) {
            for (auto& callback : _keyEvCallbacks) {
                callback(key, scancode, action, mods);
            }
        }

        void GlfwWindowManager::fireCharEv(unsigned int codepoint) {
            for (auto& callback : _charEvCallbacks) {
                callback(codepoint);
            }
        }

        void GlfwWindowManager::fireMouseBtnEv(int button, int action, int mods) {
            for (auto& callback : _mouseBtnEvCallbacks) {
                callback(button, action, mods);
            }
        }

        void GlfwWindowManager::fireCursorPosEv(double xPos, double yPos) {
            for (auto& callback : _cursorPosEvCallbacks) {
                callback(xPos, yPos);
            }
        }

        void GlfwWindowManager::fireScrollEv(double xOffset, double yOffset) {
            for (auto& callback : _scrollEvCallbacks) {
                callback(xOffset, yOffset);
            }
        }

        void GlfwWindowManager::fireWindowSizeEv(int width, int height) {
            for (auto& callback : _windowSizeEvCallbacks) {
                callback(width, height);
            }
        }

        void GlfwWindowManager::fireWindowCloseEv() {
            for (auto& callback : _windowCloseEvCallbacks) {
                callback();
            }
        }
    }
}
