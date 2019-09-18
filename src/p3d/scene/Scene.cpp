#include "Scene.h"
#include "../common/Uuid.h"
#include "../assert.h"

namespace p3d {
    Scene::Scene(
        std::unique_ptr<SpacePartitionerI>& spacePartitioner,
        GPUResourceProviderI* resProvider
    ) : _resProvider(resProvider), _spacePartitioner(std::move(spacePartitioner)){

    }

    Scene::~Scene() {

    }

    const SceneProperties& Scene::getProperties() const {
        return _properties;
    }

    void Scene::setProperties(const SceneProperties& properties) {
        _properties = properties;
    }

    HModel Scene::create(const ModelDesc& desc) {
        if (!desc.mesh.isValid() || !desc.material.isValid()) {
            return HModel();
        }
        return _models.insert(std::unique_ptr<void*>(), desc, genUUID());
    }

    HMesh Scene::create(const MeshDesc& desc) {
        auto mesh = _resProvider->createMesh(desc);
        if (nullptr == mesh) {
            return HMesh();
        }

        return _meshes.insert(mesh, desc, genUUID());
    }

    HMaterial Scene::create(const MaterialDesc& desc) {
        //TODO do we even need to create gpu data per material?
        /*auto material = _resProvider->createMaterial(desc);
        if (nullptr == material) {
            return HMaterial();
        }*/

        return _materials.insert(std::unique_ptr<MaterialI>(), desc, genUUID());
    }

    HTexture2dArr Scene::create(const TextureDesc& desc) {
        auto tex2dArr = _resProvider->createTexture2dArray(desc);
        if (nullptr == tex2dArr) {
            return HTexture2dArr();
        }

        return _textures2dArr.insert(tex2dArr, desc, genUUID());
    }

    HVertexShader Scene::create(const VertexShaderDesc& desc) {
        auto vs = _resProvider->createVertexShader(desc);
        if (nullptr == vs) {
            return HVertexShader();
        }

        return _vertexShaders.insert(vs, desc, genUUID());
    }

    HPixelShader Scene::create(const PixelShaderDesc& desc) {
        auto ps = _resProvider->createPixelShader(desc);
        if (nullptr == ps) {
            return HPixelShader();
        }

        return _pixelShaders.insert(ps, desc, genUUID());
    }

    HLight Scene::create(const LightDesc& desc) {
        if (_lights.getAllHandles().size() == MAX_LIGHTS) {
            P3D_ERROR_PRINT("Failed to create light. Maximum number of lights reached");
            return HLight();
        }
        return _lights.insert(std::unique_ptr<void*>(), desc, genUUID());
    }

    const MeshI* Scene::get(HMesh handle) const {
        return _meshes.getResource(handle);
    }

    const MaterialI* Scene::get(HMaterial handle) const {
        return _materials.getResource(handle);
    }

    const Texture2dArrayI* Scene::get(HTexture2dArr handle) const {
        return _textures2dArr.getResource(handle);
    }

    const VertexShaderI* Scene::get(HVertexShader handle) const {
        return _vertexShaders.getResource(handle);
    }

    const PixelShaderI* Scene::get(HPixelShader handle) const {
        return _pixelShaders.getResource(handle);
    }

    const ModelDesc* Scene::getDesc(HModel handle) const {
        return _models.getDesc(handle);
    }

    const MeshDesc* Scene::getDesc(HMesh handle) const {
        return _meshes.getDesc(handle);
    }

    const MaterialDesc* Scene::getDesc(HMaterial handle) const {
        return _materials.getDesc(handle);
    }

    const TextureDesc* Scene::getDesc(HTexture2dArr handle) const {
        return _textures2dArr.getDesc(handle);
    }

    const VertexShaderDesc* Scene::getDesc(HVertexShader handle) const {
        return _vertexShaders.getDesc(handle);
    }

    const PixelShaderDesc* Scene::getDesc(HPixelShader handle) const {
        return _pixelShaders.getDesc(handle);
    }

    const LightDesc* Scene::getDesc(HLight handle) const {
        return _lights.getDesc(handle);
    }

    const std::vector<HModel>& Scene::getAllModels() const {
        return _models.getAllHandles();
    }

    const std::vector<HMesh>& Scene::getAllMeshes() const {
        return _meshes.getAllHandles();
    }

    const std::vector<HMaterial>& Scene::getAllMaterials() const {
        return _materials.getAllHandles();
    }

    const std::vector<HTexture2dArr>& Scene::getAllTexture2dArr() const {
        return _textures2dArr.getAllHandles();
    }

    const std::vector<HVertexShader>& Scene::getAllVertexShaders() const {
        return _vertexShaders.getAllHandles();
    }

    const std::vector<HPixelShader>& Scene::getAllPixelShaders() const {
        return _pixelShaders.getAllHandles();
    }

    const std::vector<HLight>& Scene::getAllLights() const {
        return _lights.getAllHandles();
    }

    const std::vector<HModel>& Scene::getVisibleModels() const {
        //TODO reimplement using space partitioner
        return _models.getAllHandles();
    }

    bool Scene::update(HModel handle, const ModelDesc& desc) {
        return _models.update(handle, std::unique_ptr<void*>(), desc);
    }

    bool Scene::update(HMaterial handle, const MaterialDesc& desc) {
        return _materials.update(handle, std::unique_ptr<MaterialI>(), desc);
    }

    bool Scene::update(HLight handle, const LightDesc& desc) {
        return _lights.update(handle, std::unique_ptr<void*>(), desc);
    }

    /*bool Scene::update(HMesh handle, const MeshDesc& desc) {
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
    }*/

    bool Scene::remove(HModel handle) {
        return _models.remove(handle);
    }

    bool Scene::remove(HMesh handle) {
        return _meshes.remove(handle);
    }

    bool Scene::remove(HMaterial handle) {
        return _materials.remove(handle);
    }

    bool Scene::remove(HTexture2dArr handle) {
        return _textures2dArr.remove(handle);
    }

    bool Scene::remove(HVertexShader handle) {
        return _vertexShaders.remove(handle);
    }

    bool Scene::remove(HPixelShader handle) {
        return _pixelShaders.remove(handle);
    }

    bool Scene::remove(HLight handle) {
        return _lights.remove(handle);
    }
}