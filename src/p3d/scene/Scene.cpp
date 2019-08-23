#include "Scene.h"
#include "../common/Uuid.h"

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
        auto tex2dArr = _resProvider->createTexture2dArray(desc);
        if (nullptr == tex2dArr) {
            return HTexture2dArr();
        }

        HResource hres = _textures2dArr.insert(tex2dArr, desc, genUUID());
        return HTexture2dArr(hres.buffPosition, hres.uuid);
    }

    HVertexShader Scene::create(const VertexShaderDesc& desc) {
        auto vs = _resProvider->createVertexShader(desc);
        if (nullptr == vs) {
            return HVertexShader();
        }

        HResource hres = _vertexShaders.insert(vs, desc, genUUID());
        return HVertexShader(hres.buffPosition, hres.uuid);
    }

    HPixelShader Scene::create(const PixelShaderDesc& desc) {
        auto ps = _resProvider->createPixelShader(desc);
        if (nullptr == ps) {
            return HPixelShader();
        }

        HResource hres = _pixelShaders.insert(ps, desc, genUUID());
        return HPixelShader(hres.buffPosition, hres.uuid);
    }

    const MeshI* Scene::get(HMesh handle) const {
        return nullptr;
    }

    const MaterialI* Scene::get(HMaterial handle) const {
        return nullptr;
    }

    const Texture2dArrayI* Scene::get(HTexture2dArr handle) const {
        return nullptr;
    }

    const VertexShaderI* Scene::get(HVertexShader handle) const {
        return nullptr;
    }

    const PixelShaderI* Scene::get(HPixelShader handle) const {
        return nullptr;
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

    /*bool Scene::update(HModel handle, const ModelDesc& desc) {
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
}