#pragma once

#include "p3d_ConsoleQueue.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include "GLFW/glfw3native.h"

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace p3d
{
	namespace util
	{
		class GlfwWindowManager
		{
		public:
			GlfwWindowManager();
			~GlfwWindowManager();

			bool initialize(
				int screenWidth,
				int screenHeight,
				bool fullscreen,
				bool decorated,
				bool resizable,
				bool enableCursor,
				std::string title,
				std::function<void(unsigned int level, std::string)> consoleOutCallback
				);

			void getClientSize(int& width, int& height);
			HWND getWindowHandle();

			void enableCursor(bool enable);
			void pollEvents();

			void addKeyEvCbk(std::function<void(int key, int scancode, int action, int mods)> callback);
			void addCharEvCbk(std::function<void(unsigned int codepoint)> callback);
			void addMouseBtnEvCbk(std::function<void(int button, int action, int mods)> callback);
			void addCursorPosEvCbk(std::function<void(double xPos, double yPos)> callback);
			void addScrollEvCbk(std::function<void(double xOffset, double yOffset)> callback);
			void addWindowSizeEvCbk(std::function<void(int width, int height)> callback);
			void addWindowCloseEvCbk(std::function<void()> callback);

			void fireKeyEv(int key, int scancode, int action, int mods);
			void fireCharEv(unsigned int codepoint);
			void fireMouseBtnEv(int button, int action, int mods);
			void fireCursorPosEv(double xPos, double yPos);
			void fireScrollEv(double xOffset, double yOffset);
			void fireWindowSizeEv(int width, int height);
			void fireWindowCloseEv();

		private:
			void setWindowCallbacks(GLFWwindow* window);
			GLFWwindow* _window;
			static void glfwErrorCallback(int error, const char* description);

			std::vector<std::function<void(int key, int scancode, int action, int mods)>> _keyEvCallbacks;
			std::vector<std::function<void(unsigned int codepoint)>> _charEvCallbacks;
			std::vector<std::function<void(int button, int action, int mods)>> _mouseBtnEvCallbacks;
			std::vector<std::function<void(double xPos, double yPos)>> _cursorPosEvCallbacks;
			std::vector<std::function<void(double xOffset, double yOffset)>> _scrollEvCallbacks;
			std::vector<std::function<void(int width, int height)>> _windowSizeEvCallbacks;
			std::vector<std::function<void()>> _windowCloseEvCallbacks;
		};
	}
}