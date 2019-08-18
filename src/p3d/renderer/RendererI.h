#pragma once

#include "Texture1dArrayI.h"
#include "Texture2dArrayI.h"
#include "Texture3dI.h"
#include "VertexShaderI.h"
#include "PixelShaderI.h"
#include "../ResourceDescriptions.h"

#include <memory>

namespace p3d {
    class RendererI {
    public:
        virtual void renderFrame() = 0;

        virtual bool createTexture1dArray(
            const TextureDesc& desc,
            std::unique_ptr <p3d::Texture1dArrayI>& tex
        ) = 0;

        virtual bool createTexture2dArray(
            const TextureDesc& desc,
            std::unique_ptr <p3d::Texture2dArrayI>& tex
        ) = 0;

        virtual bool createTexture3d(
            const TextureDesc& desc,
            std::unique_ptr <p3d::Texture3dI>& tex
        ) = 0;

        virtual bool createVertexShader(
            const VertexShaderDesc& desc,
            std::unique_ptr <p3d::VertexShaderI>& vs
        ) = 0;

        virtual bool createPixelShader(
            const PixelShaderDesc& desc,
            std::unique_ptr <p3d::PixelShaderI>& ps
        ) = 0;
    };
}
