#pragma once
#include "../dx/dx_ComPtr.h"
#include "../Texture1dArrayI.h"
#include <d3d11.h>

namespace p3d {
    namespace d3d11 {

        class Texture1dArray : public p3d::Texture1dArrayI {
        public:
            Texture1dArray();
            Texture1dArray(
                ComPtr<ID3D11Texture1D> texArr, 
                ComPtr<ID3D11DepthStencilView> depthStencilView,
                ComPtr<ID3D11RenderTargetView> renderTargetView,
                ComPtr<ID3D11ShaderResourceView> shaderResourceView,
                const TextureDesc& desc
            );
            Texture1dArray(Texture1dArray&&);
            Texture1dArray& operator=(Texture1dArray&&);

            const ComPtr<ID3D11Texture1D> getTexture() const;
            const ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
            const ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;
            const ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
            const TextureDesc& getDescription() const;

        private:
            Texture1dArray(const Texture1dArray&) = delete;
            Texture1dArray& operator=(const Texture1dArray&) = delete;

            ComPtr<ID3D11Texture1D> _texArr = nullptr;
            ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
            TextureDesc _desc;
        };
    }
}