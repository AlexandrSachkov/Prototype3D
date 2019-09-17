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
#include "p3d/scene/PerspectiveCamera.h"
#include "p3d/common/StandardShapes.h"
#include "p3d/common/AABB.h"

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
        1,  // anisotropy
        1,  // number of back buffers (1 for double buffering, 2 for tripple)
        fullscreen
    ), "Failed to initialize D3D11 device");

    std::unique_ptr<p3d::RendererI> renderer(d3d11Renderer.release());
    std::unique_ptr<p3d::SceneI> cubeScene(new p3d::Scene(
        std::unique_ptr<p3d::SpacePartitionerI>(new p3d::NullSpacePartitioner()), 
        renderer.get()
    ));

   
    p3d::MeshDesc meshDesc = p3d::shapes::createCube(); 
    p3d::HMesh hMesh = cubeScene->create(meshDesc);
    P3D_ASSERT_R(hMesh.isValid(), "Failed to create mesh");

    p3d::MaterialDesc materialDesc;
    materialDesc.diffuseColor = { 1.0f,1.0f,0.0f };
    materialDesc.opacity = 0.3f;
    p3d::HMaterial hMaterial1 = cubeScene->create(materialDesc);
    P3D_ASSERT_R(hMaterial1.isValid(), "Failed to create material1");

    materialDesc.diffuseColor = { 0.0f,1.0f,0.0f };
    p3d::HMaterial hMaterial2 = cubeScene->create(materialDesc);
    P3D_ASSERT_R(hMaterial2.isValid(), "Failed to create material2");
    
    p3d::ModelDesc modelDesc;
    modelDesc.mesh = hMesh;
    modelDesc.material = hMaterial1;
    modelDesc.boundingVolume.type = p3d::P3D_BOUNDING_VOLUME_TYPE::P3D_BOUNDING_VOLUME_AABB;
    modelDesc.boundingVolume.volume.aabb = p3d::AABB::build(meshDesc.vertices.get(), meshDesc.verticesSize, modelDesc.transform);
    modelDesc.drawBoundingVolume = true;
    P3D_ASSERT_R(cubeScene->create(modelDesc).isValid(), "Failed to create model1");

    modelDesc.material = hMaterial2;
    modelDesc.transform = glm::translate(glm::mat4x4(), { 0,0,-2 });
    modelDesc.boundingVolume.volume.aabb = p3d::AABB::build(meshDesc.vertices.get(), meshDesc.verticesSize, modelDesc.transform);
    P3D_ASSERT_R(cubeScene->create(modelDesc).isValid(), "Failed to create model2");
    //modelDesc.transform = glm::rotate(modelDesc.transform, glm::radians(45.0f), { 0,1,0 });
    //modelDesc.transform = glm::translate(modelDesc.transform, { -1,2,-3 });

    

    std::unique_ptr<p3d::PerspectiveCamera> camera(
        new p3d::PerspectiveCamera({ 0,0,2 }, 45.0f, (float)clientDim[0] / clientDim[1], 0.05f, 1000.0f)
    );

    sampleRunner.setRunProcedure([&]() {
        renderer->renderFrame(cubeScene.get(), camera.get());
    });
    sampleRunner.setCameraController(camera.get(), 0.001f, 0.001f);
    sampleRunner.start();

    return true;
}

int main() {
    if (run()) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
