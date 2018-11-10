#pragma once

#include "../RenderingDevice.h"
#include "../../math/Vec2.h"
#include "../../ResourceDescriptions.h"
#include "../Texture1dArrayI.h"
#include "../Texture2dArrayI.h"
#include "../Texture3dI.h"
#include "../VertexShaderI.h"
#include "../PixelShaderI.h"
#include "../BufferI.h"
#include "../dx/ComPtr.h"
#include "d3d11_Texture2dArray.h"

#include "glm/vec4.hpp"

#include <d3d11.h>

#include <memory>

namespace p3d {
    namespace d3d11 {
        class RenderingDevice : public p3d::RenderingDevice<p3d::d3d11::RenderingDevice> {
        public:
            RenderingDevice();
            ~RenderingDevice();

            bool initialize(
                HWND windowHandle,
                unsigned int screenRefreshRate,
                p3d::Vec2_uint screenDim,
                unsigned int msaaLevel,
                unsigned int numBackBuffers,
                bool fullscreen
            );
            void release();

            p3d::Texture2dArrayI& getRenderTargetBuff();
            p3d::Texture2dArrayI& getDepthStencilBuff();

            bool clearRenderTargetBuff(const p3d::Texture2dArrayI* renderTargetBuff, glm::vec4 color);
            bool clearDepthBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth);
            bool clearStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, unsigned int stencil);
            bool clearDepthStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth, unsigned int stencil);

            bool OMSetRenderTargets(
                const p3d::Texture2dArrayI* renderTargetBuff,
                const p3d::Texture2dArrayI* depthStencilBuff
            );

            bool VSSetShader(const p3d::VertexShaderI* vs);
            bool PSSetShader(const p3d::PixelShaderI* ps);

            bool IASetVertexBuffer(
                const p3d::BufferI* vBuff,
                unsigned int offset,
                unsigned int slot
            );

            void presentFrame();

            bool createTexture1dArray(
                const Texture1dArrayDesc& desc,
                std::unique_ptr <p3d::Texture1dArrayI>& tex
            );

            bool createTexture2dArray(
                const Texture2dArrayDesc& desc,
                std::unique_ptr <p3d::Texture2dArrayI>& tex
            );

            bool createTexture3d(
                const Texture3dDesc& desc,
                std::unique_ptr <p3d::Texture3dI>& tex
            );

            bool createVertexShader(
                const VertexShaderDesc& desc,
                std::unique_ptr <p3d::VertexShaderI>& vs
            );

            bool createPixelShader(
                const PixelShaderDesc& desc,
                std::unique_ptr <p3d::PixelShaderI>& ps
            );

            bool createBuffer(
                const BufferDesc& desc,
                std::unique_ptr <p3d::BufferI>& buffer
            );

        private:
            RenderingDevice(RenderingDevice const&) = delete;
            RenderingDevice(RenderingDevice&&) = delete;
            RenderingDevice& operator=(RenderingDevice const&) = delete;
            RenderingDevice& operator=(RenderingDevice &&) = delete;

            bool createInputLayout(
                const std::vector<VertexShaderDesc::InputElementDesc>& inputDesc,
                ComPtr<ID3DBlob> vsBlob,
                ComPtr<ID3D11InputLayout>& inputLayout
            );

            ComPtr<ID3D11Device> _device = nullptr;
            ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
            ComPtr<IDXGISwapChain> _swapChain = nullptr;

            Texture2dArray _renderTargetBuff;
            Texture2dArray _depthStencilBuff;
        };
    }
}