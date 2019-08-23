#pragma once

#include <memory>
#include "../ResourceDescriptions.h"
#include "MeshI.h"
#include "MaterialI.h"
#include "Texture1dArrayI.h"
#include "Texture2dArrayI.h"
#include "Texture3dI.h"
#include "VertexShaderI.h"
#include "PixelShaderI.h"

namespace p3d {
    class GPUResourceProviderI {
    public:
        virtual ~GPUResourceProviderI() {}

        virtual std::unique_ptr<p3d::MeshI> createMesh(
            const MeshDesc& desc
        ) = 0;

        virtual std::unique_ptr<p3d::MaterialI> createMaterial(
            const MaterialDesc& desc
        ) = 0;

        virtual std::unique_ptr<p3d::Texture1dArrayI> createTexture1dArray(
            const TextureDesc& desc
        ) = 0;

        virtual std::unique_ptr <p3d::Texture2dArrayI> createTexture2dArray(
            const TextureDesc& desc
        ) = 0;

        virtual std::unique_ptr <p3d::Texture3dI> createTexture3d(
            const TextureDesc& desc
        ) = 0;

        virtual std::unique_ptr <p3d::VertexShaderI> createVertexShader(
            const VertexShaderDesc& desc
        ) = 0;

        virtual std::unique_ptr <p3d::PixelShaderI> createPixelShader(
            const PixelShaderDesc& desc
        ) = 0;
    };
}