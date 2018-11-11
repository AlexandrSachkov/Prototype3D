#include "d3d11_RenderingDevice.h"
#include "../../assert.h"
#include "../dx/dx_ConstConvert.h"
#include "d3d11_ConstConvert.h"
#include "d3d11_Utility.h"

#include "d3d11_Texture1dArray.h"
#include "d3d11_Texture2dArray.h"
#include "d3d11_Texture3d.h"
#include "d3d11_VertexShader.h"
#include "d3d11_PixelShader.h"
#include "d3d11_Buffer.h"
#include "d3d11_Rasterizer.h"

#include <Windows.h>

#include <algorithm>

namespace p3d {
    namespace d3d11 {
        RenderingDevice::RenderingDevice() {

        }

        RenderingDevice::~RenderingDevice() {
            release();
        }

        bool RenderingDevice::initialize(
            HWND windowHandle,
            unsigned int screenRefreshRate,
            unsigned int screenDim[2],
            unsigned int msaaLevel,
            unsigned int numBackBuffers,
            bool fullscreen
        ) {
            P3D_ASSERT_R(Utility::createDevice(_device, _deviceContext), "d3d11 device failed to initialize");

            if (msaaLevel < 1) msaaLevel = 1; //anything less than 1 is considered off. So we set the sampling count to 1

            unsigned int maxMsaaQualityLvl = 0;
            if (msaaLevel > 1) {
                P3D_ASSERT_R(Utility::getMSAAQualityLevel(_device, msaaLevel, maxMsaaQualityLvl), "Failed to retrieve MSAA quality level");
            }

            unsigned int msaaQualityLevel = maxMsaaQualityLvl - 1;

            P3D_ASSERT_R(Utility::createSwapChain(
                windowHandle,
                screenRefreshRate,
                screenDim,
                msaaLevel,
                msaaQualityLevel,
                numBackBuffers,
                fullscreen,
                _device,
                _swapChain
            ), "Failed to create swap chain");

            ComPtr<ID3D11Texture2D> backBuffRenderTarget = nullptr;
            ComPtr<ID3D11RenderTargetView> backBuffRenderTargetView = nullptr;
            P3D_ASSERT_R(Utility::createBackBufferRenderTargetView(
                _device,
                _swapChain,
                backBuffRenderTarget,
                backBuffRenderTargetView
            ), "Failed to create back buffer render target view");
            _renderTargetBuff = Texture2dArray(
                backBuffRenderTarget,
                nullptr,
                backBuffRenderTargetView,
                nullptr,
                nullptr,
                {} //TODO: add a description
            );


            ComPtr<ID3D11Texture2D> depthStencilBuff = nullptr;
            ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
            P3D_ASSERT_R(Utility::createDepthStencilView(
                _device,
                screenDim,
                msaaLevel,
                msaaQualityLevel,
                depthStencilBuff,
                depthStencilView
            ), "Failed to create depth stencil view");
            _depthStencilBuff = Texture2dArray(
                depthStencilBuff,
                depthStencilView,
                nullptr,
                nullptr,
                nullptr,
                {} //TODO: add a description
            );

            return true;
        }

        void RenderingDevice::release() {
            if (_swapChain) Utility::setFullScreen(_swapChain, false);

            _swapChain = nullptr;
            _deviceContext = nullptr;
            _device = nullptr;
        }

        p3d::Texture2dArrayI& RenderingDevice::getRenderTargetBuff() {
            return _renderTargetBuff;
        }

        p3d::Texture2dArrayI& RenderingDevice::getDepthStencilBuff() {
            return _depthStencilBuff;
        }

        bool RenderingDevice::clearRenderTargetBuff(const p3d::Texture2dArrayI* renderTargetBuff, float color[4]) {
            const d3d11::Texture2dArray* rtBuff = static_cast<const d3d11::Texture2dArray*>(renderTargetBuff);
            P3D_ASSERT_R(rtBuff->getRenderTargetView(), "Render target view is NULL");

            _deviceContext->ClearRenderTargetView(rtBuff->getRenderTargetView().Get(), color);
            return true;
        }

        bool RenderingDevice::clearDepthBuff(const p3d::Texture2dArrayI* depthStencilBuff, float depth) {
            const d3d11::Texture2dArray* dsBuff = static_cast<const d3d11::Texture2dArray*>(depthStencilBuff);
            P3D_ASSERT_R(dsBuff->getDepthStencilView(), "Depth stencil view is NULL");

            _deviceContext->ClearDepthStencilView(dsBuff->getDepthStencilView().Get(), D3D11_CLEAR_DEPTH, depth, 0);
            return true;
        }

        bool RenderingDevice::clearStencilBuff(const p3d::Texture2dArrayI* depthStencilBuff, unsigned int stencil) {
            const d3d11::Texture2dArray* dsBuff = static_cast<const d3d11::Texture2dArray*>(depthStencilBuff);
            P3D_ASSERT_R(dsBuff->getDepthStencilView(), "Depth stencil view is NULL");

            _deviceContext->ClearDepthStencilView(
                dsBuff->getDepthStencilView().Get(), D3D11_CLEAR_STENCIL, 0.0f, stencil
            );
            return true;
        }

        bool RenderingDevice::clearDepthStencilBuff(
            const p3d::Texture2dArrayI* depthStencilBuff,
            float depth,
            unsigned int stencil) {

            const d3d11::Texture2dArray* dsBuff = static_cast<const d3d11::Texture2dArray*>(depthStencilBuff);
            P3D_ASSERT_R(dsBuff->getDepthStencilView(), "Depth stencil view is NULL");

            _deviceContext->ClearDepthStencilView(
                dsBuff->getDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil
            );
            return true;
        }

        bool RenderingDevice::OMSetRenderTargets(
            const p3d::Texture2dArrayI* renderTargetBuff,
            const p3d::Texture2dArrayI* depthStencilBuff
        ) {
            const d3d11::Texture2dArray* rtBuff = static_cast<const d3d11::Texture2dArray*>(renderTargetBuff);
            P3D_ASSERT_R(rtBuff->getRenderTargetView(), "Render target view is NULL");
            const d3d11::Texture2dArray* dsBuff = static_cast<const d3d11::Texture2dArray*>(depthStencilBuff);
            P3D_ASSERT_R(dsBuff->getDepthStencilView(), "Depth stencil view is NULL");

            ID3D11RenderTargetView* rtView = rtBuff->getRenderTargetView().Get();
            _deviceContext->OMSetRenderTargets(1, &rtView, dsBuff->getDepthStencilView().Get());
            return true;
        }

        bool RenderingDevice::IASetPrimitiveTopology(P3D_PRIMITIVE_TOPOLOGY tp) {
            _deviceContext->IASetPrimitiveTopology(convertPrimitiveTopology(tp));
            return true;
        }

        bool RenderingDevice::VSSetShader(const p3d::VertexShaderI* vs) {
            const d3d11::VertexShader* d3d11vs = static_cast<const d3d11::VertexShader*>(vs);
            _deviceContext->VSSetShader(const_cast<ID3D11VertexShader*>(d3d11vs->getShader().Get()), nullptr, 0);
            _deviceContext->IASetInputLayout(const_cast<ID3D11InputLayout*>(d3d11vs->getInputLayout().Get()));
            return true;
        }

        bool RenderingDevice::PSSetShader(const p3d::PixelShaderI* ps) {
            const d3d11::PixelShader* d3d11ps = static_cast<const d3d11::PixelShader*>(ps);
            _deviceContext->PSSetShader(const_cast<ID3D11PixelShader*>(d3d11ps->getShader().Get()), nullptr, 0);
            return true;
        }

        bool RenderingDevice::RSSetState(const p3d::RasterizerI* rast) {
            const d3d11::Rasterizer* d3d11rast = static_cast<const d3d11::Rasterizer*>(rast);
            _deviceContext->RSSetState(const_cast<ID3D11RasterizerState*>(d3d11rast->getRasterizer().Get()));
            return true;
        }

        void RenderingDevice::RSSetViewport(float topLeft[2], float dimensions[2], float minMaxDepth[2]) {
            Utility::setViewPort(_deviceContext, topLeft, dimensions, minMaxDepth);
        }

        bool RenderingDevice::IASetVertexBuffer(
            const p3d::BufferI* vBuff,
            unsigned int offset,
            unsigned int slot
        ) {
            const d3d11::Buffer* d3d11buff = static_cast<const d3d11::Buffer*>(vBuff);
            ID3D11Buffer* nativeBuff = d3d11buff->getBuffer().Get();
            _deviceContext->IASetVertexBuffers(slot, 1, &nativeBuff, (UINT*)&d3d11buff->getDescription().strideBytes, (UINT*)&offset);
            return true;
        }

        void RenderingDevice::draw(unsigned int vertexCount, unsigned int vertexStartLocation) {
            _deviceContext->Draw(vertexCount, vertexStartLocation);
        }

        void RenderingDevice::presentFrame() {
            _swapChain->Present(0, 0);
        }

        bool RenderingDevice::createTexture1dArray(
            const Texture1dArrayDesc& desc,
            std::unique_ptr <p3d::Texture1dArrayI>& tex) {
            //TODO: create GPU texture
            tex.reset(new Texture1dArray({ nullptr }, desc));
            return true;
        }

        bool RenderingDevice::createTexture2dArray(
            const Texture2dArrayDesc& desc,
            std::unique_ptr <p3d::Texture2dArrayI>& tex) {

            //TODO: create GPU texture array
            tex.reset(new Texture2dArray(
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                desc
            ));
            return true;
        }

        bool RenderingDevice::createTexture3d(
            const Texture3dDesc& desc,
            std::unique_ptr <p3d::Texture3dI>& tex) {
            //TODO: create GPU texture array
            tex.reset(new Texture3d({ nullptr }, desc));
            return true;
        }

        bool RenderingDevice::createVertexShader(
            const VertexShaderDesc& desc,
            std::unique_ptr <p3d::VertexShaderI>& vs) {

            ComPtr<ID3DBlob> blob = nullptr;
            ComPtr<ID3DBlob> errBlob = nullptr;
            P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "vs_5_0", blob, errBlob),
                "Failed to compile vertex shader: " + std::string((char*)errBlob->GetBufferPointer())
            );

            ComPtr<ID3D11VertexShader> shader = nullptr;
            P3D_ASSERT_R(Utility::createVertexShader(_device, blob, shader), "Failed to create vertex shader");

            ComPtr<ID3D11InputLayout> inputLayout = nullptr;
            P3D_ASSERT_R(createInputLayout(desc.inputDesc, blob, inputLayout), "Failed to create input layout");

            vs.reset(new VertexShader(shader, blob, inputLayout, desc));
            return true;
        }

        bool RenderingDevice::createPixelShader(
            const PixelShaderDesc& desc,
            std::unique_ptr <p3d::PixelShaderI>& ps) {

            ComPtr<ID3DBlob> blob = nullptr;
            ComPtr<ID3DBlob> errBlob = nullptr;
            P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "ps_5_0", blob, errBlob),
                "Failed to compile pixel shader: " + std::string((char*)errBlob->GetBufferPointer())
            );

            ComPtr<ID3D11PixelShader> shader = nullptr;
            P3D_ASSERT_R(Utility::createPixelShader(_device, blob, shader), "Failed to create pixel shader");

            ps.reset(new PixelShader(shader, blob, desc));
            return true;
        }

        bool RenderingDevice::createBuffer(
            const BufferDesc& desc,
            std::unique_ptr <p3d::BufferI>& buffer
        ) {
            unsigned int bindFlags = 0;
            for (auto bindFlag : desc.bindFlags) {
                D3D11_BIND_FLAG d3dBindFlag;
                P3D_ASSERT_R(convertBindFlag(bindFlag, d3dBindFlag),
                    "Failed to convert bind flag to d3d11 equivalent");

                bindFlags |= d3dBindFlag;
            }
            UsageDesc usageDesc;
            P3D_ASSERT_R(convertUsageFlag(desc.usageFlag, usageDesc),
                "Failed to convert usage flag to d3d11 equivalent");

            const unsigned int buffSize = desc.strideBytes * desc.length;
            P3D_ASSERT_R(!((!desc.data || buffSize ==0) && usageDesc.usage == D3D11_USAGE_IMMUTABLE),
                "Data must be provided for an immutable buffer");

            ComPtr<ID3D11Buffer> buff = nullptr;
            P3D_ASSERT_R(Utility::createBuffer(
                _device,
                bindFlags,
                usageDesc.usage,
                usageDesc.cpuAccessFlag,
                desc.data,
                buffSize,
                buff
            ), "Failed to create buffer");

            buffer.reset(new Buffer(buff, desc));
            return true;
        }

        bool RenderingDevice::createRasterizer(
            const RasterizerDesc& desc,
            std::unique_ptr <p3d::RasterizerI>& rasterizer
        ) {
            D3D11_CULL_MODE d3d11Cull;
            P3D_ASSERT_R(convertCullMode(desc.cullMode, d3d11Cull), 
                "Failed to convert cull mode to d3d11 equivalent");

            D3D11_FILL_MODE d3d11Fill;
            P3D_ASSERT_R(convertFillMode(desc.fillMode, d3d11Fill),
                "Failed to convert fill mode to d3d11 equivalent");

            ComPtr<ID3D11RasterizerState> rs = nullptr;
            P3D_ASSERT_R(Utility::createRasterizerState(
                _device,
                d3d11Cull,
                d3d11Fill,
                desc.frontCounterClockwise,
                rs
            ), "Failed to create rasterizer state");

            rasterizer.reset(new Rasterizer(rs, desc));
            return true;
        }

        bool RenderingDevice::createInputLayout(
            const std::vector<VertexShaderDesc::InputElementDesc>& inputDesc,
            ComPtr<ID3DBlob> vsBlob,
            ComPtr<ID3D11InputLayout>& inputLayout
        ) {
            std::vector<VertexShaderDesc::InputElementDesc> p3dInputDescTemp(
                inputDesc.begin(), inputDesc.end()
            );

            //sort by input slot in assending order
            std::sort(p3dInputDescTemp.begin(), p3dInputDescTemp.end(),
                [](VertexShaderDesc::InputElementDesc i, VertexShaderDesc::InputElementDesc j) {
                return i.inputSlot < j.inputSlot;
            }
            );

            unsigned int currInputSlot = 0;
            unsigned int currOffset = 0;
            std::vector<D3D11_INPUT_ELEMENT_DESC> elementDesc;
            for (int i = 0; i < p3dInputDescTemp.size(); i++) {// auto p3dElemDesc : p3dInputDescTemp) {
                auto p3dElemDesc = p3dInputDescTemp[i];
                if (currInputSlot != p3dElemDesc.inputSlot) {
                    currInputSlot = p3dElemDesc.inputSlot;
                    currOffset = 0;
                }
                elementDesc.push_back({
                    p3dInputDescTemp[i].elementName.c_str(),
                    0,
                    dx::convertFormat(p3dElemDesc.dataFormat),
                    currInputSlot,
                    currOffset,
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                    });
                currOffset += p3dElemDesc.dataSizeBytes;
            }

            return Utility::createInputLayout(_device, vsBlob, elementDesc, inputLayout);
        }
    }
}