#include "p3d/util/util_DefaultSampleRunner.h"

#include "p3d/assert.h"
#include "p3d/device/d3d11/d3d11_RenderingDevice.h"
#include "p3d/device/RenderingDeviceI.h"
#include "p3d/device/Texture1dArrayI.h"
#include "p3d/device/Texture2dArrayI.h"
#include "p3d/device/Texture3dI.h"
#include "p3d/device/VertexShaderI.h"
#include "p3d/device/PixelShaderI.h"
#include "p3d/Constants.h"

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

    std::unique_ptr<p3d::RenderingDeviceI> device = nullptr;
    p3d::d3d11::RenderingDevice* d3d11_device = new p3d::d3d11::RenderingDevice();
    P3D_ASSERT_R(d3d11_device->initialize(
        sampleRunner.getWindowHandle(),
        windowDim,
        60, // refresh rate
        1,  // multi-sampling
        1,  // number of back buffers (1 for double buffering, 2 for tripple)
        fullscreen
    ), "Failed to initialize D3D11 device");
    device.reset(d3d11_device);

    sampleRunner.setRunProcedure([&]() {
        device->renderFrame();
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
