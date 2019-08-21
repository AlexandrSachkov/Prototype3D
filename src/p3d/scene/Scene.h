#pragma once

#include "SceneI.h"
#include "ResourceBank.h"
#include "../renderer/GPUResourceProviderI.h"
#include "SpacePartitionerI.h"

namespace p3d {
    class Scene : public SceneI {
    public:
        Scene(
            GPUResourceProviderI* resProvider,
            std::unique_ptr<SpacePartitionerI> spacePartitioner
        );
        ~Scene();

        HModel create(const ModelDesc& desc) override;
        HMesh create(const MeshDesc& desc) override;
        HMaterial create(const MaterialDesc& desc) override;
        HTexture2dArr create(const TextureDesc& desc) override;
        HVertexShader create(const VertexShaderDesc& desc) override;
        HPixelShader create(const PixelShaderDesc& desc) override;

        const ModelDesc* get(HModel handle) const override;
        const MeshDesc* get(HMesh handle) const override;
        const MaterialDesc* get(HMaterial handle) const override;
        const TextureDesc* get(HTexture2dArr handle) const override;
        const VertexShaderDesc* get(HVertexShader handle) const override;
        const PixelShaderDesc* get(HPixelShader handle) const override;

        bool update(HModel handle, const ModelDesc& desc) override;
        bool update(HMesh handle, const MeshDesc& desc) override;
        bool update(HMaterial handle, const MaterialDesc& desc) override;
        bool update(HTexture2dArr handle, const TextureDesc& desc) override;
        bool update(HVertexShader handle, const VertexShaderDesc& desc) override;
        bool update(HPixelShader handle, const PixelShaderDesc& desc) override;

        bool remove(HModel handle) override;
        bool remove(HMesh handle) override;
        bool remove(HMaterial handle) override;
        bool remove(HTexture2dArr handle) override;
        bool remove(HVertexShader handle) override;
        bool remove(HPixelShader handle) override;

    private:
        GPUResourceProviderI* _resProvider;
        std::unique_ptr<SpacePartitionerI> _spacePartitioner;
    };
}