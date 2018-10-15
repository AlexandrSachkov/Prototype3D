#include "util/p3d_GlfwWindowManager.h"
#include "util/p3d_Timer.h"

#include "assert.h"
#include "device/RenderingDeviceI.h"
#include "device/GPUMemoryManagerI.h"
#include "device/Texture2dArray.h"

#include <memory>
#include <assert.h>

bool run() {
	bool fullscreen = false;
	int winWidth = 800, winHeight = 600;
	std::string winTitle = "Prototype3D test";

	auto windowManager = p3d::util::GlfwWindowManager();
	P3D_ASSERT_R(windowManager.initialize(winWidth, winHeight, fullscreen, true, false, true, winTitle,
		[&](unsigned int level, std::string data) {
		//consoleQueue.push((p3d::util::CONSOLE_OUT_TYPE)level, data);
	}), "Failed to initialize window manager");

	p3d::RenderingDeviceI* device = nullptr;
	p3d::GPUMemoryManagerI* memMngr = nullptr;

#ifdef P3D_API_D3D11
	p3d::d3d11::RenderingDevice& d3d11_device = p3d::d3d11::RenderingDevice::instance();
	P3D_ASSERT_R(d3d11_device.initialize(
		windowManager.getWindowHandle(),
		60,
		{ (unsigned int)winWidth, (unsigned int)winHeight },
		4,
		1,
		false
	), "Failed to initialize D3D11 device");
	device, memMngr = &d3d11_device;
#endif

	p3d::Texture1dArray* texArr1d = nullptr;
	p3d::Texture2dArray* texArr2d = nullptr;
	P3D_ASSERT_R(memMngr->createTexture1dArray(10, texArr1d), "Failed to create 1d texture");
	P3D_ASSERT_R(memMngr->createTexture2dArray({ 5,5 }, 10, texArr2d), "Failed to create 2d texture");



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

	do {
		windowManager.pollEvents();

	} while (running);

	return true;
}

int main()
{
	if (run()) {
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
