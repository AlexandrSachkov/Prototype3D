#include "p3d/util/util_DefaultSampleRunner.h"

#include "p3d/assert.h"
#include "p3d/renderer/d3d11/d3d11_Renderer.h"
#include "p3d/renderer/RendererI.h"
#include "p3d/renderer/Texture1dArrayI.h"
#include "p3d/renderer/Texture2dArrayI.h"
#include "p3d/renderer/Texture3dI.h"
#include "p3d/renderer/VertexShaderI.h"
#include "p3d/renderer/PixelShaderI.h"
#include "p3d/Constants.h"
#include "p3d/scene/Scene.h"
#include "p3d/scene/NullSpacePartitioner.h"
#include "p3d/util/util_SceneImporter.h"
#include "p3d/scene/PerspectiveCamera.h"

#include "glm/vec3.hpp"

#include <memory>
#include <string>

bool run() {
    const bool fullscreen = false;
    const unsigned int windowDim[] = { 800, 600 };
    const std::string winTitle = "Prototype3D sample 1: Create window";

    auto sampleRunner = p3d::util::DefaultSampleRunner();
    P3D_ASSERT_R(sampleRunner.initialize(winTitle, windowDim, fullscreen, true, false, true),
        "Failed to initialize window manager");

    std::unique_ptr <p3d::d3d11::Renderer> d3d11Renderer(new p3d::d3d11::Renderer());
    P3D_ASSERT_R(d3d11Renderer->initialize(
        sampleRunner.getWindowHandle(),
        windowDim,
        60, // refresh rate
        1,  // multi-sampling
        1,  // number of back buffers (1 for double buffering, 2 for tripple)
        fullscreen
    ), "Failed to initialize D3D11 device");

    std::unique_ptr<p3d::RendererI> renderer(d3d11Renderer.release());
    std::unique_ptr<p3d::SceneI> cubeScene(new p3d::Scene(
        std::unique_ptr<p3d::SpacePartitionerI>(new p3d::NullSpacePartitioner()), 
        renderer.get()
    ));

    P3D_ASSERT_R(
        p3d::util::SceneImporter::import("D:/Repositories/Prototype3D/resources/cube/cube.obj", "", cubeScene.get(), nullptr),
        "Failed to import landlord.dae");

    std::unique_ptr<p3d::PerspectiveCamera> camera(
        new p3d::PerspectiveCamera({ 0,0,-2 }, 45.0f, (float)windowDim[0] / windowDim[1], 0.05f, 1000.0f)
    );

    sampleRunner.setRunProcedure([&]() {
        renderer->renderFrame(cubeScene.get(), camera.get());
    });

    float moveStepSize = 0.05f;
    sampleRunner.addKeyEvCbk([&](int key, int scancode, int action, int mod) {
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->move(moveStepSize, 0.0f);
        } else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->move(-moveStepSize, 0.0f);
        } else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->move(0.0f, -moveStepSize);
        } else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->move(0.0f, moveStepSize);
        }
    });
    sampleRunner.start();

    return true;
}

int main() {
    if (run()) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
