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
        1,  // anisotropy
        1,  // number of back buffers (1 for double buffering, 2 for tripple)
        fullscreen
    ), "Failed to initialize D3D11 device");

    std::unique_ptr<p3d::RendererI> renderer(d3d11Renderer.release());
    std::unique_ptr<p3d::SceneI> scene(new p3d::Scene(
        std::unique_ptr<p3d::SpacePartitionerI>(new p3d::NullSpacePartitioner()), 
        renderer.get()
    ));

    sampleRunner.setRunProcedure([&]() {
        renderer->renderFrame(scene.get(), nullptr);
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
