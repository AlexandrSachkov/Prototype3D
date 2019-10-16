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
    const unsigned int windowDim[] = { 1920, 1080 };
    const std::string winTitle = "Prototype3D sample 1: Create window";

    auto sampleRunner = p3d::util::DefaultSampleRunner();
    P3D_ASSERT_R(sampleRunner.initialize(winTitle, windowDim, fullscreen, true, false, false),
        "Failed to initialize window manager");

    unsigned int clientDim[2];
    sampleRunner.getClientSize(clientDim);

    std::unique_ptr <p3d::d3d11::Renderer> d3d11Renderer(new p3d::d3d11::Renderer());
    P3D_ASSERT_R(d3d11Renderer->initialize(
        sampleRunner.getWindowHandle(),
        clientDim,
        60, // refresh rate
        1,  // multi-sampling
        16,  // anisotropy
        1,  // number of back buffers (1 for double buffering, 2 for tripple)
        fullscreen
    ), "Failed to initialize D3D11 device");

    std::unique_ptr<p3d::RendererI> renderer(d3d11Renderer.release());
    std::unique_ptr<p3d::SceneI> sponzaScene(new p3d::Scene(
        std::unique_ptr<p3d::SpacePartitionerI>(new p3d::NullSpacePartitioner()), 
        renderer.get()
    ));

    P3D_ASSERT_R(
        p3d::util::SceneImporter::import("D:/Repositories/Prototype3D/resources/sponza/sponza.obj", "", sponzaScene.get(), nullptr),
        "Failed to import cube.obj");

    p3d::SceneProperties sceneProperties = sponzaScene->getProperties();
    sceneProperties.ambientLight = { 0.2f,0.2f,0.2f };
    sceneProperties.drawBoundingVolumes = false;
    sponzaScene->setProperties(sceneProperties);

    //Field of view should be 45 degrees. Otherwise, it will cause artifacts (need to look into perspective distortion)
    //Keep near/far planes closer together to avoid floating precision issues
    std::unique_ptr<p3d::PerspectiveCamera> camera(
        new p3d::PerspectiveCamera({ 0,0,-2 }, 45.0f, (float)clientDim[0] / clientDim[1], 1.0f, 10000.0f)
    );

    p3d::LightDesc lightDesc;
    lightDesc.type = p3d::P3D_LIGHT_TYPE::P3D_POINT_LIGHT;
    lightDesc.desc.point.range = 10000.0f;
    lightDesc.desc.point.constAttenuation = 0.0f;
    lightDesc.desc.point.linearAttenuation = 0.005f;
    lightDesc.desc.point.quadraticAttenuation = 0.0f;
    lightDesc.desc.point.diffuseColor = { 1.0f, 1.0f, 1.0f };
    lightDesc.desc.point.specularColor = { 1.0f, 1.0f, 1.0f };
    lightDesc.desc.point.position = camera->getEye();

    p3d::HLight hlight = sponzaScene->create(lightDesc);
    P3D_ASSERT_R(hlight.isValid(), "Failed to create light");

    sampleRunner.setRunProcedure([&]() {
        lightDesc.desc.point.position = camera->getEye();
        sponzaScene->update(hlight, lightDesc);
        renderer->renderFrame(sponzaScene.get(), camera.get());
    });
    sampleRunner.setCameraController(camera.get(), 0.2f, 0.001f);
    sampleRunner.enableDebugging(sponzaScene.get());
    sampleRunner.start();

    return true;
}

int main() {
    if (run()) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
