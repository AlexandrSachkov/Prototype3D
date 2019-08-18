#include "d3d11_Texture3d.h"

namespace p3d {
    namespace d3d11 {
        Texture3d::Texture3d() {}
        Texture3d::Texture3d(
            ComPtr<ID3D11Texture3D> tex,
            ComPtr<ID3D11DepthStencilView> depthStencilView,
            ComPtr<ID3D11RenderTargetView> renderTargetView,
            ComPtr<ID3D11ShaderResourceView> shaderResourceView,
            ComPtr<ID3D11UnorderedAccessView> unorderedAccessView,
            const TextureDesc& desc
        ) : 
            _tex(tex), 
            _depthStencilView(depthStencilView),
            _renderTargetView(renderTargetView),
            _shaderResourceView(shaderResourceView),
            _unorderedAccessView(unorderedAccessView),
            _desc(desc) 
        {}

        Texture3d::Texture3d(Texture3d&& other) {
            _tex = other._tex;
            _depthStencilView = other._depthStencilView;
            _renderTargetView = other._renderTargetView;
            _shaderResourceView = other._shaderResourceView;
            _unorderedAccessView = other._unorderedAccessView;
            _desc = other._desc;

            other._tex = nullptr;
            other._depthStencilView = nullptr;
            other._renderTargetView = nullptr;
            other._shaderResourceView = nullptr;
            other._unorderedAccessView = nullptr;
            other._desc = {};
        }

        Texture3d& Texture3d::operator=(Texture3d&& other) {
            _tex = other._tex;
            _depthStencilView = other._depthStencilView;
            _renderTargetView = other._renderTargetView;
            _shaderResourceView = other._shaderResourceView;
            _unorderedAccessView = other._unorderedAccessView;
            _desc = other._desc;

            other._tex = nullptr;
            other._depthStencilView = nullptr;
            other._renderTargetView = nullptr;
            other._shaderResourceView = nullptr;
            other._unorderedAccessView = nullptr;
            other._desc = {};

            return *this;
        }

        const ComPtr<ID3D11Texture3D> Texture3d::getTexture() const {
            return _tex;
        }

        const ComPtr<ID3D11DepthStencilView> Texture3d::getDepthStencilView() const {
            return _depthStencilView;
        }

        const ComPtr<ID3D11RenderTargetView> Texture3d::getRenderTargetView() const {
            return _renderTargetView;
        }

        const ComPtr<ID3D11ShaderResourceView> Texture3d::getShaderResourceView() const {
            return _shaderResourceView;
        }

        const ComPtr<ID3D11UnorderedAccessView> Texture3d::getUnorderedAccessView() const {
            return _unorderedAccessView;
        }

        const TextureDesc& Texture3d::getDescription() const {
            return _desc;
        }
    }
}