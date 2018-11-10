#pragma once

#include "Texture1dArrayI.h"
#include "Texture2dArrayI.h"
#include "Texture3dI.h"
#include "VertexShaderI.h"
#include "PixelShaderI.h"
#include "BufferI.h"

#include "glm/vec4.hpp"

#include <memory>

namespace p3d {

    template<class T>
    class RenderingDevice {
    public:
        p3d::Texture2dArrayI& getRenderTargetBuff() {
            return static_cast<T*>(this)->getRenderTargetBuff();
        }

        p3d::Texture2dArrayI& getDepthStencilBuff() {
            return static_cast<T*>(this)->getDepthStencilBuff();
        }

        bool clearRenderTargetBuff(const p3d::Texture2dArrayI* renderTargetBuff, glm::vec4 color) {
            return static_cast<T*>(this)->clearRenderTargetBuff(renderTargetBuff, color);
        }

        bool clearDepthBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth) {
            return static_cast<T*>(this)->clearDepthBuff(depthStencilBuff, depth);
        }

        bool clearStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, unsigned int stencil) {
            return static_cast<T*>(this)->clearStencilBuff(depthStencilBuff, stencil);
        }

        bool clearDepthStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth, unsigned int stencil) {
            return static_cast<T*>(this)->clearDepthStencilBuff(depthStencilBuff, depth, stencil);
        }

        bool OMSetRenderTargets(
            const p3d::Texture2dArrayI* renderTargetBuff,
            const p3d::Texture2dArrayI* depthStencilBuff
        ) {
            return static_cast<T*>(this)->OMSetRenderTargets(renderTargetBuff, depthStencilBuff);
        }

        void presentFrame() {
            return static_cast<T*>(this)->presentFrame();
        }

        bool createTexture1dArray(
            const Texture1dArrayDesc& desc,
            std::unique_ptr <p3d::Texture1dArrayI>& tex) {
            return static_cast<T*>(this)->createTexture1dArray(desc, tex);
        }

        bool createTexture2dArray(
            const Texture2dArrayDesc& desc,
            std::unique_ptr <p3d::Texture2dArrayI>& tex) {
            return static_cast<T*>(this)->createTexture2dArray(desc, tex);
        }

        bool createTexture3d(
            const Texture3dDesc& desc,
            std::unique_ptr <p3d::Texture3dI>& tex) {
            return static_cast<T*>(this)->createTexture3d(desc, tex);
        }

        bool createVertexShader(
            const VertexShaderDesc& desc,
            std::unique_ptr <p3d::VertexShaderI>& vs) {
            return static_cast<T*>(this)->createVertexShader(desc, vs);
        }

        bool createPixelShader(
            const PixelShaderDesc& desc,
            std::unique_ptr <p3d::PixelShaderI>& ps) {
            return static_cast<T*>(this)->createPixelShader(desc, ps);
        }

        bool createBuffer(
            const BufferDesc& desc,
            const void* data,
            unsigned int sizeBytes,
            std::unique_ptr <p3d::BufferI>& buffer
        ) {
            return static_cast<T*>(this)->createBuffer(desc, data, sizeBytes, buffer);
        }

        template <typename Data>
        bool createBuffer(
            const BufferDesc& desc,
            const std::vector<Data>& data,
            std::unique_ptr <p3d::BufferI>& buffer
        ) {
            return static_cast<T*>(this)->createBuffer(desc, data.data(), (unsigned int)(sizeof(Data) * data.size()), buffer);
        }
    };
}



