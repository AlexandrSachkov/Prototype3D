#include "d3d11_Renderer.h"
#include "../../assert.h"
#include "../../common/Utils.h"
#include "../../common/StandardShapes.h"
#include "../dx/dx_ConstConvert.h"
#include "d3d11_ConstConvert.h"
#include "d3d11_Utility.h"

#include "d3d11_Mesh.h"
#include "d3d11_Material.h"
#include "d3d11_Texture1dArray.h"
#include "d3d11_Texture2dArray.h"
#include "d3d11_Texture3d.h"
#include "d3d11_VertexShader.h"
#include "d3d11_PixelShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Windows.h>
#include "DirectXMath.h"

#include <algorithm>

namespace p3d {
    namespace d3d11 {
        Renderer::Renderer() {}

        Renderer::~Renderer() {
            if (_swapChain) Utility::setFullScreen(_swapChain, false);

            _swapChain = nullptr;
            _deviceContext = nullptr;
            _device = nullptr;
        }

        bool Renderer::initialize(
            HWND windowHandle,
            const unsigned int screenDim[2],
            unsigned int screenRefreshRate,
            unsigned int msaaLevel,
            unsigned int numBackBuffers,
            bool fullscreen
        ) {
            P3D_ASSERT_R(Utility::createDevice(_device, _deviceContext), "d3d11 device failed to initialize");

            if (msaaLevel < 1) msaaLevel = 1; //anything less than 1 is considered off. So we set the sampling count to 1

            unsigned int msaaQualityLevel = 0;
            if (msaaLevel > 1) {
                unsigned int maxMsaaQualityLvl;
                P3D_ASSERT_R(Utility::getMSAAQualityLevel(_device, DXGI_FORMAT_R8G8B8A8_UNORM, msaaLevel, maxMsaaQualityLvl), 
                    "Failed to retrieve MSAA quality level");
                msaaQualityLevel = maxMsaaQualityLvl - 1;
            }

            _msaaLevel = msaaLevel;
            _msaaQualityLevel = msaaQualityLevel;

            P3D_ASSERT_R(Utility::createSwapChain(
                windowHandle,
                screenDim,
                screenRefreshRate,
                msaaLevel,
                msaaQualityLevel,
                numBackBuffers,
                fullscreen,
                _device,
                _swapChain
            ), "Failed to create swap chain");

            ComPtr<ID3D11Texture2D> backBuffRenderTarget = nullptr;
            P3D_ASSERT_R(Utility::getBackBuffer(_swapChain, backBuffRenderTarget), 
                "Failed to get back buffer");
            ComPtr<ID3D11RenderTargetView> backBuffRenderTargetView = nullptr;
            P3D_ASSERT_R(Utility::createRenderTargetView(
                _device,
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
            P3D_ASSERT_R(Utility::createTexture2DArray(_device,
                screenDim,
                1,
                1,
                false,
                msaaLevel,
                msaaQualityLevel,
                DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
                D3D11_USAGE_DEFAULT,
                0,
                D3D11_BIND_DEPTH_STENCIL,
                {},
                depthStencilBuff
            ), "Failed to create depth stencil buffer");

            ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
            P3D_ASSERT_R(Utility::createDepthStencilView(
                _device,
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

            D3D11_DEPTH_STENCIL_DESC dsDesc;
            ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

            // Depth test parameters
            dsDesc.DepthEnable = true;
            dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

            // Stencil test parameters
            dsDesc.StencilEnable = true;
            dsDesc.StencilReadMask = 0xFF;
            dsDesc.StencilWriteMask = 0xFF;

            // Stencil operations if pixel is front-facing
            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            // Stencil operations if pixel is back-facing
            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            // Create depth stencil state
            P3D_ASSERT_R_DX11(_device->CreateDepthStencilState(&dsDesc, &_defaultDepthStencilState));
            _deviceContext->OMSetDepthStencilState(_defaultDepthStencilState.Get(), 1);

            P3D_ASSERT_R(initializeRendering(screenDim), "Failed to initialize rendering");
            P3D_ASSERT_R(initializeStandardShapes(), "Failed to initialize standard shapes");

            return true;
        }

        bool Renderer::initializeRendering(const unsigned int screenDim[2]) {
            auto renderTargetView = _renderTargetBuff.getRenderTargetView().Get();
            _deviceContext->OMSetRenderTargets(1, &renderTargetView, _depthStencilBuff.getDepthStencilView().Get());

            float topLeft[2] = { 0.0f, 0.0f };
            float fScreenDim[2] = { (float)screenDim[0], (float)screenDim[1] };
            float minMaxDepth[2] = { 0.0f, 1.0f };
            Utility::setViewPort(_deviceContext, topLeft, fScreenDim, minMaxDepth);

            // transforms z-coords from -1 to 1 range (OpenGl) to 0 to 1 for DirectX
            glm::mat4x4 scale = glm::scale(glm::mat4x4(), glm::vec3(1.0f, 1.0f, 0.5f));
            glm::mat4x4 trans = glm::translate(glm::mat4x4(), glm::vec3(0, 0, 0.5f));
            _dxClipTransform = trans * scale;

            P3D_ASSERT_R(Utility::createBlendState(_device.Get(), false, false, _noBlendState), 
                "Failed to create no-blend state");

            bool fontCCW = true; //this is to enable OpenGL compatibility
            P3D_ASSERT_R(Utility::createRasterizerState(_device.Get(), D3D11_CULL_BACK, D3D11_FILL_SOLID, fontCCW, _backFaceCull),
                "Failed to create back face cull state");
            P3D_ASSERT_R(Utility::createRasterizerState(_device.Get(), D3D11_CULL_FRONT, D3D11_FILL_SOLID, fontCCW, _frontFaceCull),
                "Failed to create front face cull state");
            P3D_ASSERT_R(Utility::createRasterizerState(_device.Get(), D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, fontCCW, _wireframeMode),
                "Failed to create wireframe mode");

            for (unsigned int i = 0; i < P3D_TEX_MAP_MODE_SIZE; i++) {
                D3D11_TEXTURE_ADDRESS_MODE dx11MapMode;
                P3D_ASSERT_R(convertTextureMapMode((P3D_TEX_MAP_MODE)i, dx11MapMode), "Failed to convert texture map mode");
                P3D_ASSERT_R(Utility::createTextureSamplerState(_device.Get(), dx11MapMode, D3D11_FILTER_MIN_MAG_MIP_LINEAR /*D3D11_FILTER_ANISOTROPIC*/,
                    1, _samplerStates[i]), "Failed to create texture sampler");
            }

            dx::TransformData transformData;
            P3D_ASSERT_R(Utility::createBuffer(
                _device.Get(),
                D3D11_BIND_CONSTANT_BUFFER,
                D3D11_USAGE_DYNAMIC,
                D3D11_CPU_ACCESS_WRITE,
                &transformData,
                sizeof(dx::TransformData),
                _transformBuff
            ),"Failed to create transformation buffer");

            dx::MaterialData materialData;
            P3D_ASSERT_R(Utility::createBuffer(
                _device.Get(),
                D3D11_BIND_CONSTANT_BUFFER,
                D3D11_USAGE_DYNAMIC,
                D3D11_CPU_ACCESS_WRITE,
                &materialData,
                sizeof(dx::MaterialData),
                _materialBuff
            ), "Failed to create material buffer");

            dx::SceneConstants sceneConstants;
            P3D_ASSERT_R(Utility::createBuffer(
                _device.Get(),
                D3D11_BIND_CONSTANT_BUFFER,
                D3D11_USAGE_DYNAMIC,
                D3D11_CPU_ACCESS_WRITE,
                &sceneConstants,
                sizeof(dx::SceneConstants),
                _sceneConstantsBuff
            ), "Failed to create scene constants buffer");

            //TODO load state per material type
            // loading material-specific stuff

            VertexShaderDesc vsDesc;
            vsDesc.shaderEntryPoint = "main";
            vsDesc.inputDesc = { 
                {"POSITION", P3D_VECTOR_FORMAT::P3D_FORMAT_R32G32B32_FLOAT, P3D_VB_VERTEX_CHANNEL },
                {"NORMAL", P3D_VECTOR_FORMAT::P3D_FORMAT_R32G32B32_FLOAT, P3D_VB_NORMAL_CHANNEL },
                {"TEXCOORD", P3D_VECTOR_FORMAT::P3D_FORMAT_R32G32_FLOAT, P3D_VB_TEXCOORD_CHANNEL }
            };
            P3D_ASSERT_R(util::readFile("D:/Repositories/Prototype3D/src/p3d/renderer/shaders/hlsl/uber_vs.hlsl", vsDesc.hlslSource),
                "Failed to read vertex shader source");

            PixelShaderDesc psSource;
            psSource.shaderEntryPoint = "main";
            P3D_ASSERT_R(util::readFile("D:/Repositories/Prototype3D/src/p3d/renderer/shaders/hlsl/uber_ps.hlsl", psSource.hlslSource),
                "Failed to read pixel shader source");
            
            _vertexShader = createVertexShader(vsDesc);
            P3D_ASSERT_R(_vertexShader, "Failed to create vertex shader");
            _pixelShader = createPixelShader(psSource);
            P3D_ASSERT_R(_pixelShader, "Failed to create pixel shader");

            //This should not be done here but for every object material type during rendering
            P3D_ASSERT_R(VSSetShader(_vertexShader.get()), "Failed to set vertex shader");
            P3D_ASSERT_R(PSSetShader(_pixelShader.get()), "Failed to set pixel shader");

            float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
            _deviceContext->OMSetBlendState(_noBlendState.Get(), blendFactor, 0xffffffff);

            auto* transformBuff = _transformBuff.Get();
            _deviceContext->VSSetConstantBuffers(P3D_VS_CB_TRANSFORM_CHANNEL, 1, &transformBuff);
            auto* sceneConstantsBuff = _sceneConstantsBuff.Get();
            _deviceContext->PSSetConstantBuffers(P3D_PS_CB_SCENE_CONSTANTS_CHANNEL, 1, &sceneConstantsBuff);
            auto* materialBuff = _materialBuff.Get();
            _deviceContext->PSSetConstantBuffers(P3D_PS_CB_MATERIAL_CHANNEL, 1, &materialBuff);

            return true;
        }

        bool Renderer::initializeStandardShapes() {
            //cube
            _cubeMeshDesc = shapes::createCube();
            auto mesh = createMesh(_cubeMeshDesc);
            P3D_ASSERT_R(mesh, "Failed to create cube mesh");
            _cubeMesh.reset(static_cast<d3d11::Mesh*>(mesh.release()));
            
            return true;
        }

        p3d::Texture2dArrayI& Renderer::getRenderTargetBuff() {
            return _renderTargetBuff;
        }

        p3d::Texture2dArrayI& Renderer::getDepthStencilBuff() {
            return _depthStencilBuff;
        }

        bool Renderer::OMSetRenderTargets(
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

        bool Renderer::IASetPrimitiveTopology(P3D_PRIMITIVE_TOPOLOGY tp) {
            _deviceContext->IASetPrimitiveTopology(convertPrimitiveTopology(tp));
            return true;
        }

        bool Renderer::VSSetShader(const p3d::VertexShaderI* vs) {
            const d3d11::VertexShader* d3d11vs = static_cast<const d3d11::VertexShader*>(vs);
            _deviceContext->VSSetShader(const_cast<ID3D11VertexShader*>(d3d11vs->getShader().Get()), nullptr, 0);
            _deviceContext->IASetInputLayout(const_cast<ID3D11InputLayout*>(d3d11vs->getInputLayout().Get()));
            return true;
        }

        bool Renderer::PSSetShader(const p3d::PixelShaderI* ps) {
            const d3d11::PixelShader* d3d11ps = static_cast<const d3d11::PixelShader*>(ps);
            _deviceContext->PSSetShader(const_cast<ID3D11PixelShader*>(d3d11ps->getShader().Get()), nullptr, 0);
            return true;
        }

        /*bool RenderingDevice::RSSetState(const p3d::RasterizerI* rast) {
            const d3d11::Rasterizer* d3d11rast = static_cast<const d3d11::Rasterizer*>(rast);
            _deviceContext->RSSetState(const_cast<ID3D11RasterizerState*>(d3d11rast->getRasterizer().Get()));
            return true;
        }*/

        /*bool RenderingDevice::IASetVertexBuffer(
            const p3d::BufferI* vBuff,
            unsigned int offset,
            unsigned int slot
        ) {
            const d3d11::Buffer* d3d11buff = static_cast<const d3d11::Buffer*>(vBuff);
            ID3D11Buffer* nativeBuff = d3d11buff->getBuffer().Get();
            unsigned int elemSizeBits = getVecFormatSizeBits(d3d11buff->getDescription().dataFormat);
            P3D_ASSERT_R(elemSizeBits > 0, "Unable to get format size");

            unsigned int elemSizeBytes = elemSizeBits / 8;
            unsigned int offsetBytes = offset * elemSizeBytes;
            _deviceContext->IASetVertexBuffers(slot, 1, &nativeBuff, (UINT*)&elemSizeBytes, (UINT*)&offsetBytes);
            return true;
        }

        bool RenderingDevice::IASetIndexBuffer(const p3d::BufferI* iBuff, unsigned int offset) {
            const d3d11::Buffer* d3d11buff = static_cast<const d3d11::Buffer*>(iBuff);
            unsigned int elemSizeBits = getVecFormatSizeBits(d3d11buff->getDescription().dataFormat);
            P3D_ASSERT_R(elemSizeBits > 0, "Unable to get format size");
            _deviceContext->IASetIndexBuffer(
                d3d11buff->getBuffer().Get(),
                dx::convertVectorFormat(d3d11buff->getDescription().dataFormat), 
                elemSizeBits / 8 * offset
            );
            return true;
        }*/

        std::unique_ptr<p3d::MeshI> Renderer::createMesh(
            const MeshDesc& desc
        ) {
            P3D_ASSERT_R(desc.vertices != nullptr && desc.verticesSize > 0, "No vertex data");

            ComPtr<ID3D11Buffer> vertexBuff = nullptr;
            ComPtr<ID3D11Buffer> indexBuff = nullptr;
            ComPtr<ID3D11Buffer> texCoordBuff = nullptr;
            ComPtr<ID3D11Buffer> normalBuff = nullptr;
            ComPtr<ID3D11Buffer> tangentBuff = nullptr;
            ComPtr<ID3D11Buffer> bitangentBuff = nullptr;

            P3D_ASSERT_R(Utility::createBuffer(
                _device,
                D3D11_BIND_VERTEX_BUFFER,
                D3D11_USAGE_IMMUTABLE,
                (D3D11_CPU_ACCESS_FLAG)0,
                desc.vertices.get(),
                desc.verticesSize * sizeof(glm::vec3),
                vertexBuff
            ), "Failed to create vertex buffer");

            if (desc.indices) {
                P3D_ASSERT_R(Utility::createBuffer(
                    _device,
                    D3D11_BIND_INDEX_BUFFER,
                    D3D11_USAGE_IMMUTABLE,
                    (D3D11_CPU_ACCESS_FLAG)0,
                    desc.indices.get(),
                    desc.indicesSize * sizeof(unsigned int),
                    indexBuff
                ), "Failed to create index buffer");
            }

            if (desc.texCoords) {
                P3D_ASSERT_R(Utility::createBuffer(
                    _device,
                    D3D11_BIND_VERTEX_BUFFER,
                    D3D11_USAGE_IMMUTABLE,
                    (D3D11_CPU_ACCESS_FLAG)0,
                    desc.texCoords.get(),
                    desc.verticesSize * sizeof(glm::vec2),
                    texCoordBuff
                ), "Failed to create texture coords buffer");
            }

            if (desc.normals) {
                P3D_ASSERT_R(Utility::createBuffer(
                    _device,
                    D3D11_BIND_VERTEX_BUFFER,
                    D3D11_USAGE_IMMUTABLE,
                    (D3D11_CPU_ACCESS_FLAG)0,
                    desc.normals.get(),
                    desc.verticesSize * sizeof(glm::vec3),
                    normalBuff
                ), "Failed to create normal buffer");
            }

            if (desc.tangents) {
                P3D_ASSERT_R(Utility::createBuffer(
                    _device,
                    D3D11_BIND_VERTEX_BUFFER,
                    D3D11_USAGE_IMMUTABLE,
                    (D3D11_CPU_ACCESS_FLAG)0,
                    desc.tangents.get(),
                    desc.verticesSize * sizeof(glm::vec3),
                    tangentBuff
                ), "Failed to create tangent buffer");
            }

            if (desc.bitangents) {
                P3D_ASSERT_R(Utility::createBuffer(
                    _device,
                    D3D11_BIND_VERTEX_BUFFER,
                    D3D11_USAGE_IMMUTABLE,
                    (D3D11_CPU_ACCESS_FLAG)0,
                    desc.bitangents.get(),
                    desc.verticesSize * sizeof(glm::vec3),
                    bitangentBuff
                ), "Failed to create bitangent buffer");
            }

            return std::unique_ptr<MeshI>(new Mesh(
                vertexBuff,
                indexBuff,
                texCoordBuff,
                normalBuff,
                tangentBuff,
                bitangentBuff,
                desc.verticesSize,
                desc.indicesSize,
                convertPrimitiveTopology(desc.topology)
            ));
        }

        std::unique_ptr<p3d::MaterialI> Renderer::createMaterial(
            const MaterialDesc& desc
        ) {
            //TODO implement material
            return std::unique_ptr<MaterialI>(new Material());;
        }

        std::unique_ptr <p3d::Texture1dArrayI> Renderer::createTexture1dArray(
            const TextureDesc& desc
        ) {
            unsigned int bindFlags = 0;
            P3D_ASSERT_R(convertBindFlags(desc.bindFlags, bindFlags),
                "Failed to convert bind flags");

            UsageDesc usageDesc;
            P3D_ASSERT_R(convertUsageFlag(desc.usageFlag, usageDesc),
                "Failed to convert usage flag to d3d11 equivalent");

            DXGI_FORMAT format;
            P3D_ASSERT_R(dx::convertFormat(desc.format, format),
                "Failed to convert format to d3d11 equivalent");

            std::vector<D3D11_SUBRESOURCE_DATA> subresData;
            P3D_ASSERT_R(fillSubresourceData(desc, subresData),
                "Failed to fill subresource data");

            ComPtr<ID3D11Texture1D> textureArr = nullptr;
            P3D_ASSERT_R(Utility::createTexture1DArray(
                _device,
                desc.surfaceMatrix[0][0].surfaceDim[0],
                (unsigned int)desc.surfaceMatrix.size(),
                (unsigned int)desc.surfaceMatrix[0].size(),
                desc.generateMipMaps,
                format,
                usageDesc.usage,
                usageDesc.cpuAccessFlag,
                (D3D11_BIND_FLAG)bindFlags,
                subresData,
                textureArr
            ), "Failed to create Texture1D array");

            ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
            ComPtr<ID3D11UnorderedAccessView> unorderedAccessView = nullptr;
            P3D_ASSERT_R(createResourceViews(
                desc.bindFlags,
                textureArr,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                unorderedAccessView
            ), "Failed to create resource views");

            return std::unique_ptr<p3d::Texture1dArrayI>(new Texture1dArray(
                textureArr,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                desc
            ));
        }

        std::unique_ptr<p3d::Texture2dArrayI> Renderer::createTexture2dArray(
            const TextureDesc& desc
        ) {

            /*ID3D11Resource* texture;
            ID3D11ShaderResourceView* srView;
            wchar_t* path = L"D:\\Repositories\\Prototype3D\\resources\\crytek-sponza\\textures\\background.dds";
            DirectX::CreateDDSTextureFromFile(
                _device.Get(),
                path,
                &texture,
                &srView
            );*/
            unsigned int bindFlags = 0;
            P3D_ASSERT_R(convertBindFlags(desc.bindFlags, bindFlags), 
                "Failed to convert bind flags");

            UsageDesc usageDesc;
            P3D_ASSERT_R(convertUsageFlag(desc.usageFlag, usageDesc),
                "Failed to convert usage flag to d3d11 equivalent");

            DXGI_FORMAT format;
            P3D_ASSERT_R(dx::convertFormat(desc.format, format), 
                "Failed to convert format to d3d11 equivalent");

            std::vector<D3D11_SUBRESOURCE_DATA> subresData;
            P3D_ASSERT_R(fillSubresourceData(desc, subresData), 
                "Failed to fill subresource data");

            ComPtr<ID3D11Texture2D> textureArr = nullptr;
            P3D_ASSERT_R(Utility::createTexture2DArray(
                _device,
                desc.surfaceMatrix[0][0].surfaceDim,
                (unsigned int)desc.surfaceMatrix.size(),
                (unsigned int)desc.surfaceMatrix[0].size(),
                desc.generateMipMaps,
                1,
                0,
                format,
                usageDesc.usage,
                usageDesc.cpuAccessFlag,
                (D3D11_BIND_FLAG)bindFlags,
                subresData,
                textureArr
            ), "Failed to create Texture2D array");

            ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
            ComPtr<ID3D11UnorderedAccessView> unorderedAccessView = nullptr;
            P3D_ASSERT_R(createResourceViews(
                desc.bindFlags,
                textureArr,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                unorderedAccessView
            ), "Failed to create resource views");

            return std::unique_ptr <p3d::Texture2dArrayI>(new Texture2dArray(
                textureArr,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                unorderedAccessView,
                desc
            ));
        }

        std::unique_ptr<p3d::Texture3dI> Renderer::createTexture3d(
            const TextureDesc& desc
        ) {
            unsigned int bindFlags = 0;
            P3D_ASSERT_R(convertBindFlags(desc.bindFlags, bindFlags),
                "Failed to convert bind flags");

            UsageDesc usageDesc;
            P3D_ASSERT_R(convertUsageFlag(desc.usageFlag, usageDesc),
                "Failed to convert usage flag to d3d11 equivalent");

            DXGI_FORMAT format;
            P3D_ASSERT_R(dx::convertFormat(desc.format, format),
                "Failed to convert format to d3d11 equivalent");

            std::vector<D3D11_SUBRESOURCE_DATA> subresData;
            P3D_ASSERT_R(fillSubresourceData(desc, subresData),
                "Failed to fill subresource data");

            ComPtr<ID3D11Texture3D> texture = nullptr;
            P3D_ASSERT_R(Utility::createTexture3D(
                _device,
                desc.surfaceMatrix[0][0].surfaceDim,
                (unsigned int)desc.surfaceMatrix[0].size(),
                desc.generateMipMaps,
                format,
                usageDesc.usage,
                usageDesc.cpuAccessFlag,
                (D3D11_BIND_FLAG)bindFlags,
                subresData,
                texture
            ), "Failed to create Texture3D");
            
            ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
            ComPtr<ID3D11UnorderedAccessView> unorderedAccessView = nullptr;
            P3D_ASSERT_R(createResourceViews(
                desc.bindFlags,
                texture,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                unorderedAccessView
            ), "Failed to create resource views");

            return std::unique_ptr<p3d::Texture3dI>( new Texture3d(
                texture,
                depthStencilView,
                renderTargetView,
                shaderResourceView,
                unorderedAccessView,
                desc
            ));
        }

        std::unique_ptr<p3d::VertexShaderI> Renderer::createVertexShader(
            const VertexShaderDesc& desc
        ) {

            ComPtr<ID3DBlob> blob = nullptr;
            ComPtr<ID3DBlob> errBlob = nullptr;
            P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "vs_5_0", blob, errBlob),
                "Failed to compile vertex shader: " + std::string((char*)errBlob->GetBufferPointer())
            );

            ComPtr<ID3D11VertexShader> shader = nullptr;
            P3D_ASSERT_R(Utility::createVertexShader(_device, blob, shader), "Failed to create vertex shader");

            ComPtr<ID3D11InputLayout> inputLayout = nullptr;
            P3D_ASSERT_R(createInputLayout(desc.inputDesc, blob, inputLayout), "Failed to create input layout");

            return std::unique_ptr<p3d::VertexShaderI>( new VertexShader(
                shader, blob, inputLayout, desc
            ));
        }

        std::unique_ptr<p3d::PixelShaderI> Renderer::createPixelShader(
            const PixelShaderDesc& desc
        ) {

            ComPtr<ID3DBlob> blob = nullptr;
            ComPtr<ID3DBlob> errBlob = nullptr;
            P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "ps_5_0", blob, errBlob),
                "Failed to compile pixel shader: " + std::string((char*)errBlob->GetBufferPointer())
            );

            ComPtr<ID3D11PixelShader> shader = nullptr;
            P3D_ASSERT_R(Utility::createPixelShader(_device, blob, shader), "Failed to create pixel shader");

            return std::unique_ptr<p3d::PixelShaderI>( new PixelShader(
                shader, blob, desc
            ));
        }

        /*bool RenderingDevice::createBuffer(
            const BufferDesc& desc,
            std::unique_ptr <p3d::BufferI>& buffer
        ) {
            unsigned int bindFlags = 0;
            P3D_ASSERT_R(convertBindFlags(desc.bindFlags, bindFlags), "Failed to convert bind flags");

            UsageDesc usageDesc;
            P3D_ASSERT_R(convertUsageFlag(desc.usageFlag, usageDesc),
                "Failed to convert usage flag to d3d11 equivalent");

            unsigned int elemSizeBits = getVecFormatSizeBits(desc.dataFormat);
            P3D_ASSERT_R(elemSizeBits > 0, "Unable to get format size");

            const unsigned int buffSize = elemSizeBits / 8 * desc.length;
            P3D_ASSERT_R(!((!desc.data || buffSize == 0) && usageDesc.usage == D3D11_USAGE_IMMUTABLE),
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
        }*/

        bool Renderer::createInputLayout(
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
            unsigned int currOffsetBytes = 0;
            std::vector<D3D11_INPUT_ELEMENT_DESC> elementDesc;
            for (int i = 0; i < p3dInputDescTemp.size(); i++) {// auto p3dElemDesc : p3dInputDescTemp) {
                auto p3dElemDesc = p3dInputDescTemp[i];
                if (currInputSlot != p3dElemDesc.inputSlot) {
                    currInputSlot = p3dElemDesc.inputSlot;
                    currOffsetBytes = 0;
                }
                elementDesc.push_back({
                    p3dInputDescTemp[i].elementName.c_str(),
                    0,
                    dx::convertVectorFormat(p3dElemDesc.dataFormat),
                    currInputSlot,
                    currOffsetBytes,
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                    });

                unsigned int elemSizeBits = getVecFormatSizeBits(p3dElemDesc.dataFormat);
                P3D_ASSERT_R(elemSizeBits > 0, "Unable to get format size");
                currOffsetBytes += elemSizeBits / 8;
            }

            return Utility::createInputLayout(_device, vsBlob, elementDesc, inputLayout);
        }

        bool Renderer::convertBindFlags(
            const std::vector<P3D_BIND_FLAG>& bindFlags, 
            unsigned int& combinedBindFlags
        ) {
            unsigned int cBindFlags = 0;
            for (auto bindFlag : bindFlags) {
                D3D11_BIND_FLAG d3dBindFlag;
                P3D_ASSERT_R(convertBindFlag(bindFlag, d3dBindFlag),
                    "Failed to convert bind flag to d3d11 equivalent");

                cBindFlags |= d3dBindFlag;
            }
            combinedBindFlags = cBindFlags;
            return true;
        }

        bool Renderer::fillSubresourceData(
            const TextureDesc& desc, 
            std::vector<D3D11_SUBRESOURCE_DATA>& subresData
        ) {
            unsigned int dataOffset = 0;
            for (auto& texMipMaps : desc.surfaceMatrix) {
                for (auto& surface : texMipMaps) {
                    P3D_ASSERT_R(dataOffset + surface.surfaceSizeBytes <= desc.dataSize,
                        "Data array out of bounds");

                    subresData.push_back({
                            desc.data.get() + dataOffset,
                            surface.rowSizeBytes,
                            surface.slice2DSizeBytes
                        });
                    dataOffset += surface.surfaceSizeBytes;
                }
            }
            return true;
        }

        bool Renderer::createResourceViews(
            const std::vector<P3D_BIND_FLAG>& bindFlags,
            const ComPtr<ID3D11Resource> resource,
            ComPtr<ID3D11DepthStencilView>& depthStencilView,
            ComPtr<ID3D11RenderTargetView>& renderTargetView,
            ComPtr<ID3D11ShaderResourceView>& shaderResourceView,
            ComPtr<ID3D11UnorderedAccessView>& unorderedAccessView
        ) {
            for (auto bindFlag : bindFlags) {
                switch (bindFlag) {
                case P3D_BIND_RENDER_TARGET:
                    P3D_ASSERT_R(Utility::createRenderTargetView(_device, resource, renderTargetView),
                        "Failed to create render target view");
                    break;
                case P3D_BIND_SHADER_RESOURCE:
                    P3D_ASSERT_R(Utility::createShaderResourceView(_device, resource, shaderResourceView),
                        "Failed to create shader resource view");
                    break;
                case P3D_BIND_UNORDERED_ACCESS:
                    P3D_ASSERT_R(Utility::createUnorderedAccessView(_device, resource, unorderedAccessView),
                        "Failed to create unordered access view");
                    break;
                default:
                    return false;
                    break;
                }
            }
            return true;
        }

        void Renderer::fillMaterialData(const MaterialDesc& desc, dx::MaterialData& dataOut) {
            dataOut.diffuseColor = desc.diffuseColor;
            dataOut.emissionColor = desc.emissionColor;
            dataOut.reflectionColor = desc.reflectionColor;
            dataOut.specularColor = desc.specularColor;
            dataOut.transparencyColor = desc.transparencyColor;

            dataOut.opacity = desc.opacity;
            dataOut.reflectivity = desc.reflectivity;
            dataOut.refracti = desc.refracti;
            dataOut.shininess = desc.shininess;
            dataOut.shininessStrength = desc.shininessStrength;

            dataOut.hasDiffuteTex = desc.diffuseTex.isValid();
            dataOut.hasEmissionTex = desc.emissionTex.isValid();
            dataOut.hasLightmapTex = desc.lightmapTex.isValid();
            dataOut.hasNormalTex = desc.normalTex.isValid();
            dataOut.hasOpacityTex = desc.opacityTex.isValid();
            dataOut.hasReflectionTex = desc.reflectionTex.isValid();
            dataOut.hasShininessTex = desc.shininessTex.isValid();
            dataOut.hasSpecularTex = desc.specularTex.isValid();
        }

        void Renderer::renderFrame(const SceneI* scene, const CameraI* camera) {
            drawScene(scene, camera);
            _swapChain->Present(0, 0);
        }

        void Renderer::drawScene(const SceneI* scene, const CameraI* camera) {
            if (scene == nullptr || camera == nullptr) {
                return;
            }

            dx::SceneConstants sceneConstants;
            sceneConstants.ambientLight = scene->getProperties().ambientLight;
            Utility::updateConstBuffer(_deviceContext, _sceneConstantsBuff, &sceneConstants, sizeof(dx::SceneConstants));

            glm::mat4x4 viewProjection = _dxClipTransform * camera->getProjection() * camera->getView();

            float backgroundColor[] = { 0.0f, 0.0f, 0.0f };
            _deviceContext->ClearRenderTargetView(_renderTargetBuff.getRenderTargetView().Get(), backgroundColor);
            _deviceContext->ClearDepthStencilView(_depthStencilBuff.getDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

            auto& models = scene->getVisibleModels();
            for (HModel hmodel : models) {
                const ModelDesc* modelDesc = scene->getDesc(hmodel); 
                if (!modelDesc) {
                    continue;
                }

                const MeshDesc* meshDesc = scene->getDesc(modelDesc->mesh);
                const d3d11::Mesh* mesh = static_cast<const d3d11::Mesh*>(scene->get(modelDesc->mesh));
                const MaterialDesc* materialDesc = scene->getDesc(modelDesc->material);
                if (!meshDesc || !mesh || !materialDesc) {
                    continue;
                }

                drawModel(scene, viewProjection, modelDesc->transform, mesh, materialDesc);
                if (scene->getProperties().drawBoundingVolumes || modelDesc->drawBoundingVolume) {
                    drawBoundingVolume(scene, viewProjection, modelDesc, mesh, materialDesc);
                }
            }
        }

        void Renderer::drawModel(
            const SceneI* scene,
            const glm::mat4x4& viewProjection,
            const glm::mat4x4& modelTransform,
            const d3d11::Mesh* mesh,
            const MaterialDesc* materialDesc
        ) {
            // apply transformation
            dx::TransformData transforms;
            transforms.world = modelTransform;
            transforms.worldInvTrans = glm::transpose(glm::inverse(modelTransform));
            transforms.wvp = viewProjection * modelTransform;
            Utility::updateConstBuffer(_deviceContext, _transformBuff, &transforms, sizeof(dx::TransformData));

            // apply mesh
            auto* vertexBuff = mesh->getVertexBuffer().Get();
            unsigned int stride = sizeof(glm::vec3);
            unsigned int offset = 0;
            _deviceContext->IASetVertexBuffers(P3D_VB_VERTEX_CHANNEL, 1, &vertexBuff, &stride, &offset);
            _deviceContext->IASetPrimitiveTopology(mesh->getPrimitiveTopology());

            auto* indexBuff = mesh->getIndexBuffer().Get();
            if (indexBuff) {
                _deviceContext->IASetIndexBuffer(indexBuff, DXGI_FORMAT_R32_UINT, 0);
            }

            auto* texCoordBuff = mesh->getTextCoordBuffer().Get();
            if (texCoordBuff) {
                stride = sizeof(glm::vec2);
                offset = 0;
                _deviceContext->IASetVertexBuffers(P3D_VB_TEXCOORD_CHANNEL, 1, &texCoordBuff, &stride, &offset);
            }

            auto* normalBuff = mesh->getNormalBuffer().Get();
            if (normalBuff) {
                stride = sizeof(glm::vec3);
                offset = 0;
                _deviceContext->IASetVertexBuffers(P3D_VB_NORMAL_CHANNEL, 1, &normalBuff, &stride, &offset);
            }

            auto* tangentBuff = mesh->getTangentBuffer().Get();
            if (tangentBuff) {
                stride = sizeof(glm::vec3);
                offset = 0;
                _deviceContext->IASetVertexBuffers(P3D_VB_TANGENT_CHANNEL, 1, &tangentBuff, &stride, &offset);
            }

            auto* bitangentBuff = mesh->getBitangentBuffer().Get();
            if (bitangentBuff) {
                stride = sizeof(glm::vec3);
                offset = 0;
                _deviceContext->IASetVertexBuffers(P3D_VB_BITANGENT_CHANNEL, 1, &bitangentBuff, &stride, &offset);
            }

            //apply material
            dx::MaterialData materialData;
            fillMaterialData(*materialDesc, materialData);
            Utility::updateConstBuffer(_deviceContext, _materialBuff, &materialData, sizeof(dx::MaterialData));

            auto* sampler = _samplerStates[materialDesc->texMapMode].Get();
            _deviceContext->PSSetSamplers(0, 1, &sampler);

            if (materialDesc->diffuseTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->diffuseTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_DIFFUSE_CHANNEL, 1, &textureView);
            }

            if (materialDesc->emissionTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->emissionTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_EMISSION_CHANNEL, 1, &textureView);
            }

            if (materialDesc->lightmapTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->lightmapTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_LIGHTMAP_CHANNEL, 1, &textureView);
            }

            if (materialDesc->normalTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->normalTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_NORMAL_CHANNEL, 1, &textureView);
            }

            if (materialDesc->opacityTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->opacityTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_OPACITY_CHANNEL, 1, &textureView);
            }

            if (materialDesc->reflectionTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->reflectionTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_REFLECTION_CHANNEL, 1, &textureView);
            }

            if (materialDesc->shininessTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->shininessTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_SHININESS_CHANNEL, 1, &textureView);
            }

            if (materialDesc->specularTex.isValid()) {
                auto* p3dTexture2d = static_cast<const d3d11::Texture2dArray*>(scene->get(materialDesc->specularTex));
                auto* textureView = p3dTexture2d->getShaderResourceView().Get();
                _deviceContext->PSSetShaderResources(P3D_TEX_SPECULAR_CHANNEL, 1, &textureView);
            }

            if (materialDesc->wireframe) {
                _deviceContext->RSSetState(_wireframeMode.Get());
            } else {
                _deviceContext->RSSetState(_backFaceCull.Get());
            }

            //draw
            if (indexBuff) {
                _deviceContext->DrawIndexed(mesh->getNumIndexes(), 0, 0);
            } else {
                _deviceContext->Draw(mesh->getNumIndexes(), 0);
            }
        }

        void Renderer::drawBoundingVolume(
            const SceneI* scene,
            const glm::mat4x4& viewProjection,
            const ModelDesc* modelDesc,
            const d3d11::Mesh* mesh,
            const MaterialDesc* materialDesc
        ) {
            if (modelDesc->boundingVolume.type == P3D_BOUNDING_VOLUME_TYPE::P3D_BOUNDING_VOLUME_AABB) {
                const AABB& aabb = modelDesc->boundingVolume.volume.aabb;

                float xScale = glm::abs(aabb.getMaxX() - aabb.getMinX());
                float yScale = glm::abs(aabb.getMaxY() - aabb.getMinY());
                float zScale = glm::abs(aabb.getMaxZ() - aabb.getMinZ());
                glm::mat4x4 scale = glm::scale(glm::mat4x4(), { xScale, yScale, zScale });

                glm::mat4x4 translate = glm::translate(glm::mat4x4(), 
                    { modelDesc->transform[3][0], modelDesc->transform[3][1], modelDesc->transform[3][2]});

                MaterialDesc materialDesc;
                materialDesc.diffuseColor = { 1.0f,0.0f,1.0f };
                materialDesc.wireframe = true;
                drawModel(scene, viewProjection, translate * scale, _cubeMesh.get(), &materialDesc);
            }
        }
    }
}