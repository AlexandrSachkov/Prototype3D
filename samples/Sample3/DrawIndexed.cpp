#include "p3d/util/util_DefaultSampleRunner.h"

#include "p3d/assert.h"
#include "p3d/device/RenderingDeviceI.h"
#include "p3d/device/Texture1dArrayI.h"
#include "p3d/device/Texture2dArrayI.h"
#include "p3d/device/Texture3dI.h"
#include "p3d/device/VertexShaderI.h"
#include "p3d/device/PixelShaderI.h"
#include "p3d/device/BufferI.h"
#include "p3d/device/RasterizerI.h"
#include "p3d/Constants.h"

#include "p3d/util/util_DDSTextureLoader.h"

#include "glm/vec3.hpp"

#include <memory>
#include <string>

bool run() {
    const bool fullscreen = false;
    const unsigned int windowDim[] = {800, 600};
    const std::string winTitle = "Prototype3D sample 2: Triangle";

    auto sampleRunner = p3d::util::DefaultSampleRunner();
    P3D_ASSERT_R(sampleRunner.initialize(winTitle, windowDim, fullscreen, true, false, true),
        "Failed to initialize window manager");

    p3d::TextureDesc texDesc;
    p3d::util::DDSTextureLoader::loadDDSTextureFromFile("D:\\Repositories\\Prototype3D\\resources\\crytek-sponza\\textures\\background.dds", texDesc);
    texDesc.bindFlags = { p3d::P3D_BIND_SHADER_RESOURCE };
    texDesc.usageFlag = p3d::P3D_USAGE_CPU_UPDATE_GPU_RW;

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

    std::unique_ptr<p3d::Texture2dArrayI> tex = nullptr;
    device->createTexture2dArray(texDesc, tex);

    const std::string POSITION_IE = "POSITION";
    p3d::VertexShaderDesc vsDesc;
    vsDesc.shaderEntryPoint = "main";
    vsDesc.hlslSource =
        "float4 "+ vsDesc.shaderEntryPoint +"( float3 pos : " + POSITION_IE + " ) : SV_POSITION { "
        "	return float4(pos, 1.0f); "  
        "}"
        ;
    vsDesc.inputDesc = {
        { POSITION_IE, p3d::P3D_FORMAT_R32G32B32_FLOAT, 0 }
    };
    std::unique_ptr <p3d::VertexShaderI> vs = nullptr;
    P3D_ASSERT_R(device->createVertexShader(vsDesc, vs), "Failed to create vertex shader");
    device->VSSetShader(vs.get());

    p3d::PixelShaderDesc psDesc;
    psDesc.shaderEntryPoint = "main";
    psDesc.hlslSource =
        "float4 main(float4 pos : SV_POSITION) : SV_TARGET {"
        "return float4(0.0f, 0.0f, 1.0f, 1.0f);"
        "}"
        ;
    std::unique_ptr <p3d::PixelShaderI> ps = nullptr;
    P3D_ASSERT_R(device->createPixelShader(psDesc, ps), "Failed to create pixel shader");
    device->PSSetShader(ps.get());

    p3d::BufferDesc vertexBuffDesc;
    vertexBuffDesc.bindFlags = { p3d::P3D_BIND_VERTEX_BUFFER };
    vertexBuffDesc.usageFlag = p3d::P3D_USAGE_CPU_UPDATE_GPU_RW;
    vertexBuffDesc.dataFormat = p3d::P3D_FORMAT_R32G32B32_FLOAT;
    std::unique_ptr <p3d::BufferI> vertexBuff = nullptr;
    std::vector<glm::vec3> vertices = { {0,0.5f,0}, {-0.5f,0,0}, {0.5f,0,0.0f}, {0,-0.5f,0} };
    P3D_ASSERT_R(device->createBuffer(vertexBuffDesc, vertices, vertexBuff), "Failed to create vertex buffer");
    device->IASetVertexBuffer(vertexBuff.get(), 0, 0);
    device->IASetPrimitiveTopology(p3d::P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    p3d::BufferDesc indexBuffDesc;
    indexBuffDesc.bindFlags = { p3d::P3D_BIND_INDEX_BUFFER };
    indexBuffDesc.usageFlag = p3d::P3D_USAGE_CPU_UPDATE_GPU_RW;
    indexBuffDesc.dataFormat = p3d::P3D_FORMAT_R16_UINT;
    std::unique_ptr <p3d::BufferI> indexBuff = nullptr;
    std::vector<unsigned short> indices = { 0, 1, 2, 2, 1, 3 };
    P3D_ASSERT_R(device->createBuffer(indexBuffDesc, indices, indexBuff), "Failed to create index buffer");
    device->IASetIndexBuffer(indexBuff.get(), 0);

    p3d::RasterizerDesc rastDesc;
    rastDesc.cullMode = p3d::P3D_CULL_BACK;
    rastDesc.fillMode = p3d::P3D_FILL_SOLID;
    rastDesc.frontCounterClockwise = true;
    std::unique_ptr <p3d::RasterizerI> rast = nullptr;
    P3D_ASSERT_R(device->createRasterizer(rastDesc, rast), "Failed to create rasterizer");
    device->RSSetState(rast.get());

    const float topLeft[] = { 0.0f,0.0f };
    const float dimensions[] = { (float)windowDim[0], (float)windowDim[1] };
    const float minMaxDepth[] = { 0.0f, 1.0f };
    device->RSSetViewport(topLeft, dimensions, minMaxDepth);

    p3d::Texture2dArrayI& renderTargetBuff = device->getRenderTargetBuff();
    p3d::Texture2dArrayI& depthStencilBuff = device->getDepthStencilBuff();
    device->OMSetRenderTargets(&renderTargetBuff, &depthStencilBuff);

    sampleRunner.setRunProcedure([&]() {
        const float color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
        device->clearRenderTargetBuff(&renderTargetBuff, color);
        device->clearDepthStencilBuff(&depthStencilBuff, 1.0f, 0);

        device->drawIndexed(indexBuff->getDescription().length, 0, 0);
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
