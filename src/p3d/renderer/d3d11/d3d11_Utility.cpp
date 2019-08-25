#include "d3d11_Utility.h"

#include <d3dcompiler.h>
#include <assert.h>

namespace p3d {
    namespace d3d11 {

        bool Utility::createDevice(
            ComPtr<ID3D11Device>& device,
            ComPtr<ID3D11DeviceContext>& deviceContext
        ) {
            unsigned int creationFlags = 0;
        #if defined(P3D_DEBUG)
            creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

            D3D_FEATURE_LEVEL featureLevel;
            P3D_ASSERT_R_DX11(D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                0,
                creationFlags,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &device, &featureLevel, &deviceContext));

            P3D_ASSERT_R(featureLevel == D3D_FEATURE_LEVEL_11_0, "Direct3D Feature Level 11 unsupported");

            return true;
        }

        bool Utility::getMSAAQualityLevel(
            const ComPtr<ID3D11Device> device,
            unsigned int msaaLvl,
            unsigned int& qualityLvl
        ) {
            P3D_ASSERT_R_DX11(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, msaaLvl, &qualityLvl));
            P3D_ASSERT_R(qualityLvl > 0, "MSAA " << msaaLvl << " is not supported");

            return true;
        }

        bool Utility::setFullScreen(
            const ComPtr<IDXGISwapChain> swapChain,
            bool fullscreen
        ) {
            P3D_ASSERT_R_DX11(swapChain->SetFullscreenState(fullscreen, nullptr));
            return true;
        }

        bool Utility::createSwapChain(
            HWND windowHandle,
            const unsigned int screenDim[2],
            unsigned int screenRefreshRate,
            unsigned int msaaLevel,
            unsigned int msaaQualityLevel,
            unsigned int numBackBuffers,
            bool fullScreen,
            const ComPtr<ID3D11Device> device,
            ComPtr<IDXGISwapChain>& swapChain
        ) {
            DXGI_MODE_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

            bufferDesc.Width = screenDim[0];
            bufferDesc.Height = screenDim[1];
            bufferDesc.RefreshRate.Numerator = screenRefreshRate;
            bufferDesc.RefreshRate.Denominator = 1;
            bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

            swapChainDesc.BufferDesc = bufferDesc;
            swapChainDesc.SampleDesc.Count = msaaLevel;
            swapChainDesc.SampleDesc.Quality = msaaQualityLevel;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = numBackBuffers;
            swapChainDesc.OutputWindow = windowHandle;
            swapChainDesc.Windowed = true;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            swapChainDesc.Flags = 0;

            ComPtr<IDXGIDevice> dxgiDevice;
            P3D_ASSERT_R_DX11(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

            ComPtr<IDXGIAdapter> dxgiAdapter;
            P3D_ASSERT_R_DX11(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

            ComPtr<IDXGIFactory> dxgiFactory;
            P3D_ASSERT_R_DX11(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

            P3D_ASSERT_R_DX11(dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain));

            //Microsoft suggests not creating a full-screen swap chain
            //"This can reduce presentation performance if the swap chain size and the output window size do not match"
            //https://docs.microsoft.com/en-gb/windows/desktop/api/dxgi/nf-dxgi-idxgifactory-createswapchain
            if (fullScreen) {
                P3D_ASSERT_R(setFullScreen(swapChain, true), "Unable to set swapchain to full screen");
            }

            return true;
        }

        bool Utility::getBackBuffer(
            const ComPtr<IDXGISwapChain> swapChain,
            ComPtr<ID3D11Texture2D>& renderTargetBuff
        ) {
            P3D_ASSERT_R_DX11(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &renderTargetBuff));
            return true;
        }

        bool Utility::createRenderTargetView(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3D11Resource> resource,
            ComPtr<ID3D11RenderTargetView>& renderTargetView
        ) {
            P3D_ASSERT_R_DX11(device->CreateRenderTargetView(resource.Get(), nullptr, &renderTargetView));
            return true;
        }

        bool Utility::createDepthStencilView(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3D11Resource> resource,
            ComPtr<ID3D11DepthStencilView>& depthStencilView
        ) {
            P3D_ASSERT_R_DX11(device->CreateDepthStencilView(resource.Get(), nullptr, &depthStencilView));
            return true;
        }

        bool Utility::createShaderResourceView(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3D11Resource> resource,
            ComPtr<ID3D11ShaderResourceView>& shaderResourceView
        ) {
            P3D_ASSERT_R_DX11(device->CreateShaderResourceView(resource.Get(), nullptr, &shaderResourceView));
            return true;
        }

        bool Utility::createUnorderedAccessView(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3D11Resource> resource,
            ComPtr<ID3D11UnorderedAccessView>& unorderedAccessView
        ) {
            P3D_ASSERT_R_DX11(device->CreateUnorderedAccessView(resource.Get(), nullptr, &unorderedAccessView));
            return true;
        }

        bool Utility::compileShader(
            const std::string source,
            const std::string entryPoint,
            const std::string target,
            ComPtr<ID3DBlob>& blob,
            ComPtr<ID3DBlob>& errBlob
        ) {
            UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        #if defined( P3D_DEBUG )
            flags |= D3DCOMPILE_DEBUG;
        #endif

            P3D_ASSERT_R_DX11(D3DCompile(
                source.data(),
                source.length(),
                nullptr,
                nullptr,
                nullptr,
                entryPoint.data(),
                target.c_str(),
                flags,
                0,
                &blob,
                &errBlob
            ));

            return true;
        }

        bool Utility::createVertexShader(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3DBlob> blob,
            ComPtr<ID3D11VertexShader>& vs
        ) {
            P3D_ASSERT_R_DX11(device->CreateVertexShader(
                blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs
            ));
            return true;
        }

        bool Utility::createPixelShader(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3DBlob> blob,
            ComPtr<ID3D11PixelShader>& ps
        ) {
            P3D_ASSERT_R_DX11(device->CreatePixelShader(
                blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps
            ));
            return true;
        }

        bool Utility::createInputLayout(
            const ComPtr<ID3D11Device> device,
            const ComPtr<ID3DBlob> vsShaderBlob,
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc,
            ComPtr<ID3D11InputLayout>& inputLayout
        ) {
            P3D_ASSERT_R_DX11(device->CreateInputLayout(
                elementDesc.data(),
                (UINT)elementDesc.size(),
                vsShaderBlob->GetBufferPointer(),
                vsShaderBlob->GetBufferSize(),
                &inputLayout
            ));
            return true;
        }

        bool Utility::createBuffer(
            const ComPtr<ID3D11Device> device,
            unsigned int bindFlags,
            D3D11_USAGE usage,
            D3D11_CPU_ACCESS_FLAG cpuAccessFlag,
            const void* data,
            unsigned int sizeBytes,
            ComPtr<ID3D11Buffer>& buffer
        ) {
            D3D11_BUFFER_DESC desc;
            desc.Usage = usage;
            desc.ByteWidth = sizeBytes;
            desc.BindFlags = bindFlags;
            desc.CPUAccessFlags = cpuAccessFlag;
            desc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA subresData;
            subresData.pSysMem = data;
            subresData.SysMemPitch = 0;
            subresData.SysMemSlicePitch = 0;

            P3D_ASSERT_R_DX11(device->CreateBuffer(
                &desc,
                data ? &subresData : nullptr,
                &buffer));
            return true;
        }

        bool Utility::createBlendState(
            ID3D11Device* device,
            bool enableBlend,
            bool enableAlphaToCoverage,
            ComPtr<ID3D11BlendState>& blendState
        ) {
            D3D11_BLEND_DESC blendDesc;
            ZeroMemory(&blendDesc, sizeof(blendDesc));

            D3D11_RENDER_TARGET_BLEND_DESC rtbd;
            ZeroMemory(&rtbd, sizeof(rtbd));

            rtbd.BlendEnable = enableBlend;
            rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
            rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
            rtbd.BlendOp = D3D11_BLEND_OP_ADD;
            rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
            rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
            rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
            rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

            blendDesc.RenderTarget[0] = rtbd;
            blendDesc.IndependentBlendEnable = false;
            blendDesc.AlphaToCoverageEnable = enableAlphaToCoverage;

            P3D_ASSERT_R_DX11(device->CreateBlendState(&blendDesc, &blendState));
            return true;
        }

        bool Utility::createShaderResourceViewFromTexture2D(
            ID3D11Device* device,
            ID3D11Texture2D* texture,
            ID3D11ShaderResourceView*& resourceView
        ) {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //changes based on the image format
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = -1;
            srvDesc.Texture2D.MostDetailedMip = 0;

            HRESULT hr;
            hr = device->CreateShaderResourceView(texture, &srvDesc, &resourceView);
            if (FAILED(hr)) {
                //CONSOLE::out(CONSOLE::ERR, L"Failed to create Shader Resource View from Texture2D");
                return false;
            }
            return true;
        }

        bool Utility::createTexture1DArray(
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
            ComPtr <ID3D11Texture1D>& texture
        ) {
            D3D11_TEXTURE1D_DESC textDesc;
            ZeroMemory(&textDesc, sizeof(textDesc));
            textDesc.Width = texWidth;
            textDesc.MipLevels = (generateMipMaps && numMipMaps == 1) ? 0 : 1;
            textDesc.ArraySize = numTextures;
            textDesc.Format = format;

            textDesc.Usage = usage;
            textDesc.BindFlags = bindFlags;
            textDesc.CPUAccessFlags = cpuAccessFlag;
            textDesc.MiscFlags = 0;

            P3D_ASSERT_R_DX11(device->CreateTexture1D(
                &textDesc,
                subresDesc.size() > 0 ? subresDesc.data() : nullptr,
                &texture
            ));
            return true;
        }

        bool Utility::createTexture2DArray(
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
            ComPtr <ID3D11Texture2D>& texture
        ) {
            D3D11_TEXTURE2D_DESC textDesc;
            ZeroMemory(&textDesc, sizeof(textDesc));
            textDesc.Width = texDim[0];
            textDesc.Height = texDim[1];
            textDesc.MipLevels = (generateMipMaps && numMipMaps == 1) ? 0 : 1;
            textDesc.ArraySize = numTextures;
            textDesc.Format = format;

            DXGI_SAMPLE_DESC sampleDesc;
            ZeroMemory(&sampleDesc, sizeof(sampleDesc));
            sampleDesc.Count = msaaLevel;
            sampleDesc.Quality = msaaQualityLevel;

            textDesc.SampleDesc = sampleDesc;
            textDesc.Usage = usage;
            textDesc.BindFlags = bindFlags;
            textDesc.CPUAccessFlags = cpuAccessFlag;
            textDesc.MiscFlags = 0;

            P3D_ASSERT_R_DX11(device->CreateTexture2D(
                &textDesc, 
                subresDesc.size() > 0 ? subresDesc.data() : nullptr, 
                &texture
            ));
            return true;
        }

        bool Utility::createTexture3D(
            const ComPtr<ID3D11Device> device,
            const unsigned int texDim[3],
            unsigned int numMipMaps,
            bool generateMipMaps,
            DXGI_FORMAT format,
            D3D11_USAGE usage,
            unsigned int cpuAccessFlag,
            D3D11_BIND_FLAG bindFlags,
            const std::vector<D3D11_SUBRESOURCE_DATA>& subresDesc,
            ComPtr <ID3D11Texture3D>& texture
        ) {
            D3D11_TEXTURE3D_DESC textDesc;
            ZeroMemory(&textDesc, sizeof(textDesc));
            textDesc.Width = texDim[0];
            textDesc.Height = texDim[1];
            textDesc.Depth = texDim[2];
            textDesc.MipLevels = (generateMipMaps && numMipMaps == 1) ? 0 : 1;
            textDesc.Format = format;

            textDesc.Usage = usage;
            textDesc.BindFlags = bindFlags;
            textDesc.CPUAccessFlags = cpuAccessFlag;
            textDesc.MiscFlags = 0;

            P3D_ASSERT_R_DX11(device->CreateTexture3D(
                &textDesc,
                subresDesc.size() > 0 ? subresDesc.data() : nullptr,
                &texture
            ));
            return true;
        }

        bool Utility::createTextureSamplerState(
            const ComPtr<ID3D11Device> device,
            D3D11_TEXTURE_ADDRESS_MODE mode,
            D3D11_FILTER filter,
            unsigned int maxAntisotropy,
            ComPtr<ID3D11SamplerState> samplerState
        ) {
            P3D_ASSERT_R(maxAntisotropy <= 16, "Max antisotropy cannot be greater than 16");

            D3D11_SAMPLER_DESC sampDesc;
            ZeroMemory(&sampDesc, sizeof(sampDesc));
            sampDesc.Filter = filter;
            sampDesc.AddressU = mode;
            sampDesc.AddressV = mode;
            sampDesc.AddressW = mode;
            sampDesc.MaxAnisotropy = maxAntisotropy;
            sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sampDesc.MinLOD = 0;
            sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

            P3D_ASSERT_R_DX11(device->CreateSamplerState(&sampDesc, &samplerState));
            return true;
        }

        bool Utility::createRasterizerState(
            const ComPtr<ID3D11Device> device,
            D3D11_CULL_MODE cullMode,
            D3D11_FILL_MODE fillMode,
            bool frontCounterClockwise,
            ComPtr<ID3D11RasterizerState>& rasterizerState
        ) {
            D3D11_RASTERIZER_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

            desc.CullMode = cullMode;
            desc.FillMode = fillMode;
            desc.FrontCounterClockwise = frontCounterClockwise;
            desc.DepthClipEnable = true;

            P3D_ASSERT_R_DX11(device->CreateRasterizerState(&desc, &rasterizerState));
            return true;
        }

        void Utility::setViewPort(
            const ComPtr<ID3D11DeviceContext> deviceContext,
            const float topLeft[2],
            const float dimensions[2],
            const float minMaxDepth[2]
        ) {
            D3D11_VIEWPORT viewport;
            ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

            viewport.TopLeftX = topLeft[0];
            viewport.TopLeftY = topLeft[1];
            viewport.Width = dimensions[0];
            viewport.Height = dimensions[1];
            viewport.MinDepth = minMaxDepth[0];
            viewport.MaxDepth = minMaxDepth[1];

            deviceContext->RSSetViewports(1, &viewport);
        }
    }
}


