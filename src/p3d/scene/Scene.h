#pragma once

#include "SceneI.h"
#include "ResourceBank.h"
#include "../renderer/GPUResourceProviderI.h"
#include "SpacePartitionerI.h"

namespace p3d {
    class Scene : public SceneI {
    public:
        Scene(
            std::unique_ptr<SpacePartitionerI>& spacePartitioner,
            GPUResourceProviderI* resProvider
        );
        ~Scene();

        HModel create(const ModelDesc& desc) override;
        HMesh create(const MeshDesc& desc) override;
        HMaterial create(const MaterialDesc& desc) override;
        HTexture2dArr create(const TextureDesc& desc) override;
        HVertexShader create(const VertexShaderDesc& desc) override;
        HPixelShader create(const PixelShaderDesc& desc) override;

        const MeshI* get(HMesh handle) const;
        const MaterialI* get(HMaterial handle) const;
        const Texture2dArrayI* get(HTexture2dArr handle) const;
        const VertexShaderI* get(HVertexShader handle) const;
        const PixelShaderI* get(HPixelShader handle) const;

        const ModelDesc* getDesc(HModel handle) const override;
        const MeshDesc* getDesc(HMesh handle) const override;
        const MaterialDesc* getDesc(HMaterial handle) const override;
        const TextureDesc* getDesc(HTexture2dArr handle) const override;
        const VertexShaderDesc* getDesc(HVertexShader handle) const override;
        const PixelShaderDesc* getDesc(HPixelShader handle) const override;

        /*bool update(HModel handle, const ModelDesc& desc) override;
        bool update(HMesh handle, const MeshDesc& desc) override;
        bool update(HMaterial handle, const MaterialDesc& desc) override;
        bool update(HTexture2dArr handle, const TextureDesc& desc) override;
        bool update(HVertexShader handle, const VertexShaderDesc& desc) override;
        bool update(HPixelShader handle, const PixelShaderDesc& desc) override;*/

        bool remove(HModel handle) override;
        bool remove(HMesh handle) override;
        bool remove(HMaterial handle) override;
        bool remove(HTexture2dArr handle) override;
        bool remove(HVertexShader handle) override;
        bool remove(HPixelShader handle) override;

    private:
        GPUResourceProviderI* _resProvider;
        std::unique_ptr<SpacePartitionerI> _spacePartitioner;

        ResourceBank<void*, ModelDesc> _models;
        ResourceBank<p3d::MeshI, MeshDesc> _meshes;
        ResourceBank<p3d::MaterialI, MaterialDesc> _materials;
        ResourceBank<p3d::Texture2dArrayI, TextureDesc> _textures2dArr;
        ResourceBank<p3d::VertexShaderI, VertexShaderDesc> _vertexShaders;
        ResourceBank<p3d::PixelShaderI, PixelShaderDesc> _pixelShaders;
    };
}