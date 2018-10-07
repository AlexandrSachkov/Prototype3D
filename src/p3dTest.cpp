#include "util/p3d_GlfwWindowManager.h"
#include "util/p3d_Timer.h"

#include <memory>
#include <assert.h>

int main()
{
	bool fullscreen = false;
	int winWidth = 800, winHeight = 600;
	std::string winTitle = "Prototype3D test";

	auto windowManager = p3d::util::GlfwWindowManager();
	if (!windowManager.initialize(winWidth, winHeight, fullscreen, true, false, true, winTitle,
		[&](unsigned int level, std::string data) {
		//consoleQueue.push((p3d::util::CONSOLE_OUT_TYPE)level, data);
	}))
		return 0;

	int clientWidth, clientHeight;
	windowManager.getClientSize(clientWidth, clientHeight);

	p3d::util::Timer timer;
	//end the program when the window is closed or an ESC key is pressed
	bool running = true;
	windowManager.addWindowCloseEvCbk([&]() {
		running = false;
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			running = false;
	});
	
	bool cursorEnabled = false, initCamPos = true;
	float movementSensitivity = 0.1f;
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		//if (!cursorEnabled && key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		//	camera.move(movementSensitivity, 0);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		//if (!cursorEnabled && key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		//	camera.move(-movementSensitivity, 0);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		//if (!cursorEnabled && key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		//	camera.move(0, -movementSensitivity);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		//if (!cursorEnabled && key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		//	camera.move(0, movementSensitivity);
	});	
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		/*if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
			cursorEnabled = cursorEnabled ? false : true;
			windowManager.enableCursor(cursorEnabled);
			if (!cursorEnabled)
				initCamPos = true;
		}	*/		
	});

	float rotationSensitivity = 0.005f;
	double lastXPos = 0, lastYPos = 0;
	windowManager.addCursorPosEvCbk([&](double xPos, double yPos) {
		if (cursorEnabled)
			return;

		if (initCamPos) {
			lastXPos = xPos;
			lastYPos = yPos;
			initCamPos = false;
		}

		double yaw = (xPos - lastXPos) * rotationSensitivity;
		double pitch = (yPos - lastYPos) * rotationSensitivity;
		lastXPos = xPos;
		lastYPos = yPos;
		//camera.rotate((float)pitch, (float)yaw);
	});

	do
	{
		double deltaT = timer.deltaT();

		windowManager.pollEvents();
		int width, height;
		windowManager.getClientSize(width, height);
		

	} while (running);

	return 0;
}
