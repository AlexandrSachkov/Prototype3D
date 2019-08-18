#pragma once

#include "../RenderingDeviceI.h"
#include "../../ResourceDescriptions.h"
#include "../Texture1dArrayI.h"
#include "../Texture2dArrayI.h"
#include "../Texture3dI.h"
#include "../VertexShaderI.h"
#include "../PixelShaderI.h"
#include "../dx/dx_ComPtr.h"
#include "d3d11_Texture2dArray.h"

#include <d3d11.h>

#include <memory>

namespace p3d {
    namespace d3d11 {
        class RenderingDevice : public p3d::RenderingDeviceI {
        public:
            RenderingDevice();
            ~RenderingDevice();

            bool initialize(
                HWND windowHandle,
                const unsigned int screenDim[2],
                unsigned int screenRefreshRate,
                unsigned int msaaLevel,
                unsigned int numBackBuffers,
                bool fullscreen
            );

            void renderFrame() override;

            bool createTexture1dArray(
                const TextureDesc& desc,
                std::unique_ptr <p3d::Texture1dArrayI>& tex
            ) override;

            bool createTexture2dArray(
                const TextureDesc& desc,
                std::unique_ptr <p3d::Texture2dArrayI>& tex
            ) override;

            bool createTexture3d(
                const TextureDesc& desc,
                std::unique_ptr <p3d::Texture3dI>& tex
            ) override;

            bool createVertexShader(
                const VertexShaderDesc& desc,
                std::unique_ptr <p3d::VertexShaderI>& vs
            ) override;

            bool createPixelShader(
                const PixelShaderDesc& desc,
                std::unique_ptr <p3d::PixelShaderI>& ps
            ) override;
        
        private:

            p3d::Texture2dArrayI& getRenderTargetBuff();
            p3d::Texture2dArrayI& getDepthStencilBuff();

            bool clearRenderTargetBuff(const p3d::Texture2dArrayI* renderTargetBuff, const float color[4]);
            bool clearDepthBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth);
            bool clearStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, unsigned int stencil);
            bool clearDepthStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth, unsigned int stencil);

            bool OMSetRenderTargets(
                const p3d::Texture2dArrayI* renderTargetBuff,
                const p3d::Texture2dArrayI* depthStencilBuff
            );

            bool IASetPrimitiveTopology(P3D_PRIMITIVE_TOPOLOGY tp);
            bool VSSetShader(const p3d::VertexShaderI* vs);
            bool PSSetShader(const p3d::PixelShaderI* ps); 
            void RSSetViewport(
                const float topLeft[2], 
                const float dimensions[2], 
                const float minMaxDepth[2]
            );

            void drawIndexed(unsigned int numIndices, unsigned int startIndex, unsigned int startVertex);
            void draw(unsigned int vertexCount, unsigned int vertexStartLocation);

            RenderingDevice(RenderingDevice const&) = delete;
            RenderingDevice(RenderingDevice&&) = delete;
            RenderingDevice& operator=(RenderingDevice const&) = delete;
            RenderingDevice& operator=(RenderingDevice &&) = delete;

            bool createInputLayout(
                const std::vector<VertexShaderDesc::InputElementDesc>& inputDesc,
                ComPtr<ID3DBlob> vsBlob,
                ComPtr<ID3D11InputLayout>& inputLayout
            );

            bool convertBindFlags(const std::vector<P3D_BIND_FLAG>& bindFlags, unsigned int& combinedBindFlags);
            bool fillSubresourceData(const TextureDesc& desc, std::vector<D3D11_SUBRESOURCE_DATA>& subresData);

            bool createResourceViews(
                const std::vector<P3D_BIND_FLAG>& bindFlags,
                const ComPtr<ID3D11Resource> resource,
                ComPtr<ID3D11DepthStencilView>& depthStencilView,
                ComPtr<ID3D11RenderTargetView>& renderTargetView,
                ComPtr<ID3D11ShaderResourceView>& shaderResourceView,
                ComPtr<ID3D11UnorderedAccessView>& unorderedAccessView
            );

            ComPtr<ID3D11Device> _device = nullptr;
            ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
            ComPtr<IDXGISwapChain> _swapChain = nullptr;

            Texture2dArray _renderTargetBuff;
            Texture2dArray _depthStencilBuff;

            unsigned int _msaaLevel = 0;
            unsigned int _msaaQualityLevel = 0;
        };
    }
}