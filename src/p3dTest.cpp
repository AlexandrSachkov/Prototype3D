#include "util/p3d_GlfwWindowManager.h"
#include "util/p3d_Timer.h"

#include "assert.h"
#include "device/RenderingDeviceI.h"
#include "device/GPUMemoryManagerI.h"

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

#ifdef P3D_API_D3D11
	p3d::d3d11::RenderingDevice& d3d11_device = p3d::d3d11::RenderingDevice::instance();
	if (!d3d11_device.initialize(
		windowManager.getWindowHandle(),
		60,
		{ (unsigned int)winWidth, (unsigned int)winHeight },
		4,
		1,
		false
	)) {
		return 0;
	}
#endif

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

	do
	{
		windowManager.pollEvents();

	} while (running);

	return 0;
}
