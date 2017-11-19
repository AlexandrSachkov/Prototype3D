#include "p3d_GlfwWindowManager.h"

#include <assert.h>

namespace p3d {
	namespace util {
		GlfwWindowManager::GlfwWindowManager() :
			_window(nullptr) {
		}

		GlfwWindowManager::~GlfwWindowManager() {
			if (_window)
				glfwDestroyWindow(_window);
			glfwTerminate();
		}


		bool GlfwWindowManager::initialize(
			int screenWidth,
			int screenHeight,
			bool fullscreen,
			bool decorated,
			bool resizable,
			bool enableCurs,
			std::string title,
			std::function<void(unsigned int level, std::string)> consoleOutCallback
		) {
			glfwSetErrorCallback(glfwErrorCallback);
			if (!glfwInit()) {
				consoleOutCallback(3, "GLFW failed to initialize");
				return false;
			}

			glfwWindowHint(GLFW_RESIZABLE, resizable);
			glfwWindowHint(GLFW_DECORATED, decorated);

			if (fullscreen) {
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				_window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
			} else {
				_window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), NULL, NULL);
			}


			if (!_window) {
				glfwTerminate();
				consoleOutCallback(3, "GLFW Window failed to initialize");
				return false;
			}
			glfwSetWindowUserPointer(_window, this);
			setWindowCallbacks(_window);
			glfwSetInputMode(_window, GLFW_CURSOR, enableCurs ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

			return true;
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

		void GlfwWindowManager::getClientSize(int& width, int& height) {
			glfwGetWindowSize(_window, &width, &height);
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
