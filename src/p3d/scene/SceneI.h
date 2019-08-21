#pragma once

#include "../ResourceHandles.h"
#include "../ResourceDescriptions.h"

namespace p3d {
    class SceneI {
    public:
        ~SceneI() {}

        virtual HModel create(const ModelDesc& desc) = 0;
        virtual HMesh create(const MeshDesc& desc) = 0;
        virtual HMaterial create(const MaterialDesc& desc) = 0;
        virtual HTexture2dArr create(const TextureDesc& desc) = 0;
        virtual HVertexShader create(const VertexShaderDesc& desc) = 0;
        virtual HPixelShader create(const PixelShaderDesc& desc) = 0;

        virtual const ModelDesc* get(HModel handle) const = 0;
        virtual const MeshDesc* get(HMesh handle) const = 0;
        virtual const MaterialDesc* get(HMaterial handle) const = 0;
        virtual const TextureDesc* get(HTexture2dArr handle) const = 0;
        virtual const VertexShaderDesc* get(HVertexShader handle) const = 0;
        virtual const PixelShaderDesc* get(HPixelShader handle) const = 0;

        virtual bool update(HModel handle, const ModelDesc& desc) = 0;
        virtual bool update(HMesh handle, const MeshDesc& desc) = 0;
        virtual bool update(HMaterial handle, const MaterialDesc& desc) = 0;
        virtual bool update(HTexture2dArr handle, const TextureDesc& desc) = 0;
        virtual bool update(HVertexShader handle, const VertexShaderDesc& desc) = 0;
        virtual bool update(HPixelShader handle, const PixelShaderDesc& desc) = 0;

        virtual bool remove(HModel handle) = 0;
        virtual bool remove(HMesh handle) = 0;
        virtual bool remove(HMaterial handle) = 0;
        virtual bool remove(HTexture2dArr handle) = 0;
        virtual bool remove(HVertexShader handle) = 0;
        virtual bool remove(HPixelShader handle) = 0;
    };
}