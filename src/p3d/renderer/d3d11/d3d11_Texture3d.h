#pragma once

#include "../Texture3dI.h"
#include "../dx/dx_ComPtr.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {

        class Texture3d : public p3d::Texture3dI {
        public:
            Texture3d();
            Texture3d(
                ComPtr<ID3D11Texture3D> tex,
                ComPtr<ID3D11DepthStencilView> depthStencilView,
                ComPtr<ID3D11RenderTargetView> renderTargetView,
                ComPtr<ID3D11ShaderResourceView> shaderResourceView,
                ComPtr<ID3D11UnorderedAccessView> unorderedAccessView,
                const TextureDesc& desc
            );
            Texture3d(Texture3d&&);
            Texture3d& operator=(Texture3d&&);

            const ComPtr<ID3D11Texture3D> getTexture() const;
            const ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
            const ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;
            const ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
            const ComPtr<ID3D11UnorderedAccessView> getUnorderedAccessView() const;
            const TextureDesc& getDescription() const;

        private:
            Texture3d(const Texture3d&) = delete;
            Texture3d& operator=(const Texture3d&) = delete;

            ComPtr<ID3D11Texture3D> _tex = nullptr;
            ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
            ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
            ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
            ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView = nullptr;
            TextureDesc _desc;
        };
    }
}