#include "util/p3d_ConsoleQueue.h"
#include "util/p3d_GlfwWindowManager.h"
#include "util/p3d_ImguiUiManager.h"
#include "util/p3d_ImguiRenderer.h"
#include "util/p3d_Timer.h"
#include "util/p3d_SceneImporter.h"

#include "p3d_ResourceModel.h"
#include "P3D_RenderingEngine.h"
#include "p3d_Camera.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <assert.h>

bool initUiManager(
	p3d::util::GlfwWindowManager& windowManager,
	p3d::util::ImguiUiManager& uiManager,
	p3d::util::ConsoleQueue& consoleQueue
	);

int main()
{
	bool fullscreen = false;
	int winWidth = 800, winHeight = 600;
	std::string winTitle = "Prototype3D test";

	p3d::util::ConsoleQueue consoleQueue;

	auto windowManager = p3d::util::GlfwWindowManager();
	if (!windowManager.initialize(winWidth, winHeight, fullscreen, true, false, false, winTitle,
		[&](unsigned int level, std::string data) {
		consoleQueue.push((p3d::util::CONSOLE_OUT_TYPE)level, data);
	}))
		return 0;

	int clientWidth, clientHeight;
	windowManager.getClientSize(clientWidth, clientHeight);

	p3d::d3d11::RenderingEngine renderingEngine;
	if (!renderingEngine.initialize(windowManager.getWindowHandle(), clientWidth, clientHeight, fullscreen))
		return 0;

	auto uiManager = p3d::util::ImguiUiManager();
	if (!initUiManager(windowManager, uiManager, consoleQueue))
		return 0;

	auto uiRenderer = p3d::util::ImguiRenderer();
	if (!uiRenderer.initialize(renderingEngine.getDevice(), renderingEngine.getDeviceContext()))
		return 0;

	renderingEngine.attachUiRenderingCallback([&](void* device, void* devCon) {
		uiRenderer.render(device, devCon, uiManager.getDrawData());
	});

	// =================== SCENE =============================================
	p3d::util::SceneImporter importer;
	p3d::model::Scene scene;
	if(!importer.import("resources/landlord/landlord.dae", scene))
		return false;
	//if (!importer.import("resources/crytek-sponza/sponzaLit.dae", scene))
	//	return false;
	//scene.materials[0].wireframe = true;

	if (!renderingEngine.load(scene))
		return false;
	//========================================================================

	p3d::Camera camera({ 0, 0, 3 });

	//SCRIPT::ScriptExecutor masterExecutor;
	//SCRIPT::Bindings::attachMaster(masterExecutor.getState());
	//auto scriptModule = LuaIntf::LuaBinding(masterExecutor.getState()).beginModule("p3d");

	//scriptModule.endModule();

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
		if (!cursorEnabled && key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
			camera.move(movementSensitivity, 0);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		if (!cursorEnabled && key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
			camera.move(-movementSensitivity, 0);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		if (!cursorEnabled && key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
			camera.move(0, -movementSensitivity);
	});
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		if (!cursorEnabled && key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
			camera.move(0, movementSensitivity);
	});	
	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
			cursorEnabled = cursorEnabled ? false : true;
			windowManager.enableCursor(cursorEnabled);
			if (!cursorEnabled)
				initCamPos = true;
		}			
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
		camera.rotate((float)pitch, (float)yaw);
	});

	do
	{
		double deltaT = timer.deltaT();

		windowManager.pollEvents();
		int width, height;
		windowManager.getClientSize(width, height);
		uiManager.update(deltaT, width, height);
		renderingEngine.render(deltaT, camera);

	} while (running);

	return 0;
}

bool initUiManager(
	p3d::util::GlfwWindowManager& windowManager,
	p3d::util::ImguiUiManager& uiManager,
	p3d::util::ConsoleQueue& consoleQueue
	)
{
	//initialize
	if (!uiManager.initialize(
		[&](unsigned int level, std::string data) {
		consoleQueue.push((p3d::util::CONSOLE_OUT_TYPE)level, data);
	},
		[&](unsigned int& level, std::string& text) -> bool {
		std::pair<p3d::util::CONSOLE_OUT_TYPE, std::string> data;
		bool hasData = consoleQueue.pop(data);
		level = data.first;
		text = data.second;

		return hasData;
	},
		[](std::string script) {
		//execute script here
	},
		100
		))
	{
		return false;
	}

	//map non-character keys
	auto& io = uiManager.getIO();
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;

	//attach event callbacks
	windowManager.addMouseBtnEvCbk([&](int button, int action, int mods) {
		auto& io = uiManager.getIO();
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			io.MouseDown[0] = action == GLFW_PRESS ? 1 : 0;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			io.MouseDown[1] = action == GLFW_PRESS ? 1 : 0;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			io.MouseDown[2] = action == GLFW_PRESS ? 1 : 0;
			break;
		}
	});

	windowManager.addScrollEvCbk([&](double xOffset, double yOffset) {
		auto& io = uiManager.getIO();
		io.MouseWheel += (float)yOffset;
	});

	windowManager.addCursorPosEvCbk([&](double xPos, double yPos) {
		auto& io = uiManager.getIO();
		io.MousePos.x = (float)xPos;
		io.MousePos.y = (float)yPos;
	});

	windowManager.addCharEvCbk([&](unsigned int c) {
		auto& io = uiManager.getIO();
		if (c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);
	});

	windowManager.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
		auto& io = uiManager.getIO();
		if (action == GLFW_PRESS)
			io.KeysDown[key] = true;
		if (action == GLFW_RELEASE)
			io.KeysDown[key] = false;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	});

	return true;
}

