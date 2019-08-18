#pragma once

#include "../Texture2dArrayI.h"
#include "../dx/dx_ComPtr.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {

        class Texture2dArray : public p3d::Texture2dArrayI {
        public:
            Texture2dArray();
            Texture2dArray(
                ComPtr<ID3D11Texture2D> texArr,
                ComPtr<ID3D11DepthStencilView> depthStencilView,
                ComPtr<ID3D11RenderTargetView> renderTargetView,
                ComPtr<ID3D11ShaderResourceView> shaderResourceView,
                ComPtr<ID3D11UnorderedAccessView> unorderedAccessView,
                const TextureDesc& desc
            );
            Texture2dArray(Texture2dArray&&);
            Texture2dArray& operator=(Texture2dArray&&);

            const ComPtr<ID3D11Texture2D> getTexture() const;
            const ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
            const ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;
            const ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
            const ComPtr<ID3D11UnorderedAccessView> getUnorderedAccessView() const;
            const TextureDesc& getDescription() const;

        private:
            Texture2dArray(const Texture2dArray&) = delete;
            Texture2dArray& operator=(const Texture2dArray&) = delete;

            ComPtr<ID3D11Texture2D> _texArr = nullptr;
            ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
            ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView = nullptr;
            TextureDesc _desc;
        };
    }
}