#pragma once

#include "../RendererI.h"
#include "../../ResourceDescriptions.h"
#include "../MeshI.h"
#include "../MaterialI.h"
#include "../Texture1dArrayI.h"
#include "../Texture2dArrayI.h"
#include "../Texture3dI.h"
#include "../VertexShaderI.h"
#include "../PixelShaderI.h"
#include "../dx/dx_ComPtr.h"
#include "d3d11_Texture2dArray.h"

#include "glm/mat4x4.hpp"
#include <d3d11.h>

#include <memory>
#include <array>

namespace p3d {
    namespace d3d11 {
        class Renderer : public p3d::RendererI {
        public:
            Renderer();
            ~Renderer();

            bool initialize(
                HWND windowHandle,
                const unsigned int screenDim[2],
                unsigned int screenRefreshRate,
                unsigned int msaaLevel,
                unsigned int numBackBuffers,
                bool fullscreen
            );

            void renderFrame(const SceneI* scene, const CameraI* camera) override;

            std::unique_ptr<p3d::MeshI> createMesh(
                const MeshDesc& desc
            ) override;

            std::unique_ptr<p3d::MaterialI> createMaterial(
                const MaterialDesc& desc
            ) override;

            std::unique_ptr<p3d::Texture1dArrayI> createTexture1dArray(
                const TextureDesc& desc
            ) override;

            std::unique_ptr<p3d::Texture2dArrayI> createTexture2dArray(
                const TextureDesc& desc
            ) override;

            std::unique_ptr<p3d::Texture3dI> createTexture3d(
                const TextureDesc& desc
            ) override;

            std::unique_ptr<p3d::VertexShaderI> createVertexShader(
                const VertexShaderDesc& desc
            ) override;

            std::unique_ptr<p3d::PixelShaderI> createPixelShader(
                const PixelShaderDesc& desc
            ) override;
        
        private:
            bool initializeRendering(const unsigned int screenDim[2]);
            void drawScene(const SceneI* scene, const CameraI* camera);

            p3d::Texture2dArrayI& getRenderTargetBuff();
            p3d::Texture2dArrayI& getDepthStencilBuff();

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

            Renderer(Renderer const&) = delete;
            Renderer(Renderer&&) = delete;
            Renderer& operator=(Renderer const&) = delete;
            Renderer& operator=(Renderer &&) = delete;

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
            glm::mat4x4 _dxClipTransform;

            //pipeline state object components
            ComPtr<ID3D11BlendState> _noBlendState = nullptr;
            ComPtr<ID3D11RasterizerState> _backFaceCull = nullptr;
            ComPtr<ID3D11RasterizerState> _frontFaceCull = nullptr;
            ComPtr<ID3D11RasterizerState> _wireframeMode = nullptr;
            std::array<ComPtr<ID3D11SamplerState>, P3D_TEX_MAP_MODE_SIZE> _samplerStates;
            ComPtr<ID3D11Buffer> _perObjBuff = nullptr;


            std::unique_ptr<VertexShaderI> _vertexShader = nullptr;
            std::unique_ptr<PixelShaderI> _pixelShader = nullptr;
        };
    }
}