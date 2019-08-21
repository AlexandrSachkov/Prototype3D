#include "Scene.h"

namespace p3d {
    Scene::Scene(
        GPUResourceProviderI* resProvider,
        std::unique_ptr<SpacePartitionerI> spacePartitioner
    ) : _resProvider(resProvider), _spacePartitioner(std::move(spacePartitioner)){

    }

    Scene::~Scene() {

    }

    HModel Scene::create(const ModelDesc& desc) {
        return HModel();
    }

    HMesh Scene::create(const MeshDesc& desc) {
        return HMesh();
    }

    HMaterial Scene::create(const MaterialDesc& desc) {
        return HMaterial();
    }

    HTexture2dArr Scene::create(const TextureDesc& desc) {
        return HTexture2dArr();
    }

    HVertexShader Scene::create(const VertexShaderDesc& desc) {
        return HVertexShader();
    }

    HPixelShader Scene::create(const PixelShaderDesc& desc) {
        return HPixelShader();
    }

    const ModelDesc* Scene::get(HModel handle) const {
        return nullptr;
    }

    const MeshDesc* Scene::get(HMesh handle) const {
        return nullptr;
    }

    const MaterialDesc* Scene::get(HMaterial handle) const {
        return nullptr;
    }

    const TextureDesc* Scene::get(HTexture2dArr handle) const {
        return nullptr;
    }

    const VertexShaderDesc* Scene::get(HVertexShader handle) const {
        return nullptr;
    }

    const PixelShaderDesc* Scene::get(HPixelShader handle) const {
        return nullptr;
    }

    bool Scene::update(HModel handle, const ModelDesc& desc) {
        return false;
    }

    bool Scene::update(HMesh handle, const MeshDesc& desc) {
        return false;
    }

    bool Scene::update(HMaterial handle, const MaterialDesc& desc) {
        return false;
    }

    bool Scene::update(HTexture2dArr handle, const TextureDesc& desc) {
        return false;
    }

    bool Scene::update(HVertexShader handle, const VertexShaderDesc& desc) {
        return false;
    }

    bool Scene::update(HPixelShader handle, const PixelShaderDesc& desc) {
        return false;
    }

    bool Scene::remove(HModel handle) {
        return false;
    }

    bool Scene::remove(HMesh handle) {
        return false;
    }

    bool Scene::remove(HMaterial handle) {
        return false;
    }

    bool Scene::remove(HTexture2dArr handle) {
        return false;
    }

    bool Scene::remove(HVertexShader handle) {
        return false;
    }

    bool Scene::remove(HPixelShader handle) {
        return false;
    }
}