#pragma once

#include "../ResourceHandles.h"
#include "../ResourceDescriptions.h"

namespace p3d {
    class MeshI;
    class MaterialI;
    class Texture2dArrayI;
    class VertexShaderI;
    class PixelShaderI;

    class SceneI {
    public:
        ~SceneI() {}

        virtual HModel create(const ModelDesc& desc) = 0;
        virtual HMesh create(const MeshDesc& desc) = 0;
        virtual HMaterial create(const MaterialDesc& desc) = 0;
        virtual HTexture2dArr create(const TextureDesc& desc) = 0;
        virtual HVertexShader create(const VertexShaderDesc& desc) = 0;
        virtual HPixelShader create(const PixelShaderDesc& desc) = 0;

        virtual const MeshI* get(HMesh handle) const = 0;
        virtual const MaterialI* get(HMaterial handle) const = 0;
        virtual const Texture2dArrayI* get(HTexture2dArr handle) const = 0;
        virtual const VertexShaderI* get(HVertexShader handle) const = 0;
        virtual const PixelShaderI* get(HPixelShader handle) const = 0;

        virtual const ModelDesc* getDesc(HModel handle) const = 0;
        virtual const MeshDesc* getDesc(HMesh handle) const = 0;
        virtual const MaterialDesc* getDesc(HMaterial handle) const = 0;
        virtual const TextureDesc* getDesc(HTexture2dArr handle) const = 0;
        virtual const VertexShaderDesc* getDesc(HVertexShader handle) const = 0;
        virtual const PixelShaderDesc* getDesc(HPixelShader handle) const = 0;

        virtual const std::vector<HModel>& getAllModels() const = 0;
        virtual const std::vector<HMesh>& getAllMeshes() const = 0;
        virtual const std::vector<HMaterial>& getAllMaterials() const = 0;
        virtual const std::vector<HTexture2dArr>& getAllTexture2dArr() const = 0;
        virtual const std::vector<HVertexShader>& getAllVertexShaders() const = 0;
        virtual const std::vector<HPixelShader>& getAllPixelShaders() const = 0;

        virtual const std::vector<HModel>& getVisibleModels() const = 0;

        virtual bool update(HModel handle, const ModelDesc& desc) = 0;
        virtual bool update(HMaterial handle, const MaterialDesc& desc) = 0;
        
        /*virtual bool update(HMesh handle, const MeshDesc& desc) = 0;
        virtual bool update(HTexture2dArr handle, const TextureDesc& desc) = 0;
        virtual bool update(HVertexShader handle, const VertexShaderDesc& desc) = 0;
        virtual bool update(HPixelShader handle, const PixelShaderDesc& desc) = 0;*/

        virtual bool remove(HModel handle) = 0;
        virtual bool remove(HMesh handle) = 0;
        virtual bool remove(HMaterial handle) = 0;
        virtual bool remove(HTexture2dArr handle) = 0;
        virtual bool remove(HVertexShader handle) = 0;
        virtual bool remove(HPixelShader handle) = 0;

        virtual const SceneProperties& getProperties() const = 0;
        virtual void setProperties(const SceneProperties& properties) = 0;
    };
}