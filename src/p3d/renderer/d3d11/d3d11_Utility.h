#ifndef NL_D3D11_UTILITY_H_
#define NL_D3D11_UTILITY_H_

#include "../../assert.h"

#include <Windows.h>
#include <d3d11.h>
#include "comdef.h"
#include "../dx/dx_ComPtr.h"

#include <string>
#include <cstdint>
#include <vector>

#define P3D_ASSERT_R_DX11(hresult)  \
	P3D_ASSERT_R(!FAILED(hresult), _com_error(hresult).ErrorMessage());

namespace p3d {
    namespace d3d11 {

        class Utility {
        public:

            static bool Utility::createDevice(
                ComPtr<ID3D11Device>& device,
                ComPtr<ID3D11DeviceContext>& deviceContext
            );

            static bool Utility::getMSAAQualityLevel(
                const ComPtr<ID3D11Device> device,
                DXGI_FORMAT format,
                unsigned int msaaLvl,
                unsigned int& qualityLvl
            );

            static bool setFullScreen(
                const ComPtr<IDXGISwapChain> swapChain,
                bool fullscreen
            );

            static bool createSwapChain(
                HWND windowHandle,
                const unsigned int screenDim[2],
                unsigned int screenRefreshRate,
                unsigned int msaaLevel, //1 or below considered off
                unsigned int msaaQualityLevel,
                unsigned int numBackBuffers,
                bool fullScreen,
                const ComPtr<ID3D11Device> device,
                ComPtr<IDXGISwapChain>& swapChain
            );

            static bool Utility::getBackBuffer(
                const ComPtr<IDXGISwapChain> swapChain,
                ComPtr<ID3D11Texture2D>& renderTargetBuff
            );

            static bool createRenderTargetView(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3D11Resource> resource,
                ComPtr<ID3D11RenderTargetView>& renderTargetView
            );

            static bool createDepthStencilView(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3D11Texture2D> resource,
                ComPtr<ID3D11DepthStencilView>& depthStencilView
            );

            static bool createShaderResourceView(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3D11Resource> resource,
                ComPtr<ID3D11ShaderResourceView>& shaderResourceView
            );

            static bool createUnorderedAccessView(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3D11Resource> resource,
                ComPtr<ID3D11UnorderedAccessView>& unorderedAccessView
            );

            static bool compileShader(
                const std::string source,
                const std::string entryPoint,
                const std::string target,
                ComPtr<ID3DBlob>& blob,
                ComPtr<ID3DBlob>& errBlob
            );

            static bool createVertexShader(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3DBlob> blob,
                ComPtr<ID3D11VertexShader>& vs
            );

            static bool createPixelShader(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3DBlob> blob,
                ComPtr<ID3D11PixelShader>& ps
            );

            static bool createInputLayout(
                const ComPtr<ID3D11Device> device,
                const ComPtr<ID3DBlob> vsShaderBlob,
                const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc,
                ComPtr<ID3D11InputLayout>& inputLayout
            );

            static bool createBuffer(
                const ComPtr<ID3D11Device> device,
                unsigned int bindFlags,
                D3D11_USAGE usage,
                D3D11_CPU_ACCESS_FLAG cpuAccessFlag,
                const void* data,
                unsigned int sizeBytes,
                ComPtr<ID3D11Buffer>& buffer
            );

            static void updateConstBuffer(
                const ComPtr<ID3D11DeviceContext> deviceContext,
                const ComPtr<ID3D11Buffer> buffer,
                void* data,
                size_t dataSize
            );

            static bool createRasterizerState(
                const ComPtr <ID3D11Device> device,
                D3D11_CULL_MODE cullMode,
                D3D11_FILL_MODE fillMode,
                bool frontCounterClockwise,
                ComPtr <ID3D11RasterizerState>& rasterizerState
            );

            static bool createBlendState(
                ID3D11Device* device,
                bool enableBlend,
                bool enableAlphaToCoverage,
                ComPtr<ID3D11BlendState>& blendState
            );

            static bool createShaderResourceViewFromTexture2D(
                ID3D11Device* device,
                ID3D11Texture2D* texture,
                ID3D11ShaderResourceView*& resourceView
            );

            static bool createTexture1DArray(
                const ComPtr<ID3D11Device> device,
                const unsigned int texWidth,
                unsigned int numTextures,
                unsigned int numMipMaps,
                bool generateMipMaps,
                DXGI_FORMAT format,
                D3D11_USAGE usage,
                unsigned int cpuAccessFlag,
                D3D11_BIND_FLAG bindFlags,
                const std::vector<D3D11_SUBRESOURCE_DATA>& subresDesc,
                ComPtr<ID3D11Texture1D>& texture
            );

            static bool createTexture2DArray(
                const ComPtr<ID3D11Device> device,
                const unsigned int texDim[2],
                unsigned int numTextures,
                unsigned int numMipMaps,
                bool generateMipMaps,
                unsigned int msaaLevel,
                unsigned int msaaQualityLevel,
                DXGI_FORMAT format,
                D3D11_USAGE usage,
                unsigned int cpuAccessFlag,
                D3D11_BIND_FLAG bindFlags,
                const std::vector<D3D11_SUBRESOURCE_DATA>& subresDesc,
                ComPtr<ID3D11Texture2D>& texture
            );

            static bool createTexture3D(
                const ComPtr<ID3D11Device> device,
                const unsigned int texDim[3],
                unsigned int numMipMaps,
                bool generateMipMaps,
                DXGI_FORMAT format,
                D3D11_USAGE usage,
                unsigned int cpuAccessFlag,
                D3D11_BIND_FLAG bindFlags,
                const std::vector<D3D11_SUBRESOURCE_DATA>& subresDesc,
                ComPtr<ID3D11Texture3D>& texture
            );

            static bool createTextureSamplerState(
                const ComPtr<ID3D11Device> device,
                D3D11_TEXTURE_ADDRESS_MODE mode,
                D3D11_FILTER filter,
                unsigned int maxAntisotropy,
                ComPtr<ID3D11SamplerState>& samplerState
            );

            static void setViewPort(
                const ComPtr<ID3D11DeviceContext> deviceContext,
                const float topLeft[2],
                const float dimensions[2],
                const float minMaxDepth[2]
            );
        };
    }
}

#endif
