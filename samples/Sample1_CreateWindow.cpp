#include "util/util_DefaultSampleRunner.h"

#include "assert.h"
#include "device/RenderingDeviceI.h"
#include "device/Texture1dArrayI.h"
#include "device/Texture2dArrayI.h"
#include "device/Texture3dI.h"
#include "device/VertexShaderI.h"
#include "device/PixelShaderI.h"
#include "device/BufferI.h"
#include "device/RasterizerI.h"
#include "Constants.h"

#include "glm/vec3.hpp"

#include <memory>
#include <string>

bool run() {
    const bool fullscreen = false;
    const unsigned int windowDim[] = { 800, 600 };
    const std::string winTitle = "Prototype3D sample 2: Triangle";

    auto sampleRunner = p3d::util::DefaultSampleRunner();
    P3D_ASSERT_R(sampleRunner.initialize(winTitle, windowDim, fullscreen, true, false, true),
        "Failed to initialize window manager");


    std::unique_ptr<p3d::RenderingDeviceI> device = nullptr;
#ifdef P3D_API_D3D11 
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
#endif

    p3d::Texture2dArrayI& renderTargetBuff = device->getRenderTargetBuff();
    p3d::Texture2dArrayI& depthStencilBuff = device->getDepthStencilBuff();
    device->OMSetRenderTargets(&renderTargetBuff, &depthStencilBuff);

    sampleRunner.setRunProcedure([&]() {
        const float color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
        device->clearRenderTargetBuff(&renderTargetBuff, color);
        device->clearDepthStencilBuff(&depthStencilBuff, 1.0f, 0);

        device->presentFrame();
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
