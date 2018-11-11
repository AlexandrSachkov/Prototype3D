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
                ComPtr<ID3D11Device> device,
                unsigned int msaaLvl,
                unsigned int& qualityLvl
            );

            static bool setFullScreen(
                ComPtr<IDXGISwapChain> swapChain,
                bool fullscreen
            );

            static bool createSwapChain(
                HWND windowHandle,
                unsigned int screenRefreshRate,
                unsigned int screenDim[2],
                unsigned int msaaLevel, //1 or below considered off
                unsigned int msaaQualityLevel,
                unsigned int numBackBuffers,
                bool fullScreen,
                ComPtr<ID3D11Device> device,
                ComPtr<IDXGISwapChain>& swapChain
            );

            static bool createBackBufferRenderTargetView(
                ComPtr<ID3D11Device> device,
                ComPtr<IDXGISwapChain> swapChain,
                ComPtr<ID3D11Texture2D>& renderTargetBuff,
                ComPtr<ID3D11RenderTargetView>& renderTargetView
            );

            static bool createDepthStencilView(
                ComPtr<ID3D11Device> device,
                unsigned int screenDim[2],
                unsigned int msaaLevel,
                unsigned int msaaQualityLevel,
                ComPtr<ID3D11Texture2D>& depthStencilBuff,
                ComPtr<ID3D11DepthStencilView>& depthStencilView
            );

            static bool compileShader(
                std::string source,
                std::string entryPoint,
                std::string target,
                ComPtr<ID3DBlob>& blob,
                ComPtr<ID3DBlob>& errBlob
            );

            static bool createVertexShader(
                ComPtr<ID3D11Device> device,
                ComPtr<ID3DBlob> blob,
                ComPtr<ID3D11VertexShader>& vs
            );

            static bool createPixelShader(
                ComPtr<ID3D11Device> device,
                ComPtr<ID3DBlob> blob,
                ComPtr<ID3D11PixelShader>& ps
            );

            static bool createInputLayout(
                ComPtr<ID3D11Device> device,
                ComPtr<ID3DBlob> vsShaderBlob,
                const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc,
                ComPtr<ID3D11InputLayout>& inputLayout
            );

            static bool createBuffer(
                ComPtr<ID3D11Device> device,
                unsigned int bindFlags,
                D3D11_USAGE usage,
                D3D11_CPU_ACCESS_FLAG cpuAccessFlag,
                const void* data,
                unsigned int sizeBytes,
                ComPtr<ID3D11Buffer>& buffer
            );

            static bool createRasterizerState(
                ComPtr <ID3D11Device> device,
                D3D11_CULL_MODE cullMode,
                D3D11_FILL_MODE fillMode,
                bool frontCounterClockwise,
                ComPtr <ID3D11RasterizerState>& rasterizerState
            );

            static bool createBlendStates(
                ID3D11Device* device,
                bool enableBlend,
                bool enableIndependentBlending,
                uint_fast32_t numRenderTargets,
                bool enableAlphaToCoverage,
                ID3D11BlendState*& blendState
            );

            static bool createShaderResourceViewFromTexture2D(
                ID3D11Device* device,
                ID3D11Texture2D* texture,
                ID3D11ShaderResourceView*& resourceView
            );

            static bool createTexture2D(
                ID3D11Device* device,
                uint_fast32_t width,
                uint_fast32_t height,
                bool generateMipMaps,
                uint_fast32_t MSAALevel,
                D3D11_USAGE usage,
                D3D11_BIND_FLAG bindFlag,
                void* data,
                uint_fast32_t memPitch,
                ID3D11Texture2D*& texture
            );

            static bool createTextureSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);

            /*static bool loadVertexShader(ID3D11Device* device, std::wstring path, RESOURCES::VertexShader& vertexShader);
            static bool loadPixelShader(ID3D11Device* device, std::wstring path, RESOURCES::PixelShader& pixelShader);
            */

            static void setViewPort(
                ComPtr<ID3D11DeviceContext> deviceContext,
                float topLeft[2],
                float dimensions[2],
                float minMaxDepth[2]
            );

            /*static void updateBuffer(
                ID3D11DeviceContext*& deviceContext,
                RESOURCES::Buffer& buffer,
                void* data,
                size_t dataSize
                );*/

        private:
            static bool compileBlobFromFile(
                std::wstring path,
                LPCSTR entryPoint,
                LPCSTR profile,
                ID3DBlob*& blob
            );

            static bool loadBlobFromFile(std::wstring path, ID3DBlob*& blob);
        };
    }
}

#endif
