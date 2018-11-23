#include "d3d11_Texture1dArray.h"

namespace p3d {
    namespace d3d11 {
        Texture1dArray::Texture1dArray() {}
        Texture1dArray::Texture1dArray(
            ComPtr<ID3D11Texture1D> texArr, 
            ComPtr<ID3D11DepthStencilView> depthStencilView,
            ComPtr<ID3D11RenderTargetView> renderTargetView,
            ComPtr<ID3D11ShaderResourceView> shaderResourceView,
            const TextureDesc& desc
        ) : _texArr(texArr), 
            _depthStencilView(depthStencilView),
            _renderTargetView(renderTargetView),
            _shaderResourceView(shaderResourceView),
            _desc(desc) 
        {}

        Texture1dArray::Texture1dArray(Texture1dArray&& other) {
            _texArr = other._texArr;
            _depthStencilView = other._depthStencilView;
            _renderTargetView = other._renderTargetView;
            _shaderResourceView = other._shaderResourceView;
            _desc = other._desc;

            other._texArr = nullptr;
            other._depthStencilView = nullptr;
            other._renderTargetView = nullptr;
            other._shaderResourceView = nullptr;
            other._desc = {};
        }

        Texture1dArray& Texture1dArray::operator=(Texture1dArray&& other) {
            _texArr = other._texArr;
            _depthStencilView = other._depthStencilView;
            _renderTargetView = other._renderTargetView;
            _shaderResourceView = other._shaderResourceView;
            _desc = other._desc;

            other._texArr = nullptr;
            other._depthStencilView = nullptr;
            other._renderTargetView = nullptr;
            other._shaderResourceView = nullptr;
            other._desc = {};

            return *this;
        }

        const ComPtr<ID3D11Texture1D> Texture1dArray::Texture1dArray::getTexture() const {
            return _texArr;
        }

        const ComPtr<ID3D11DepthStencilView> Texture1dArray::getDepthStencilView() const {
            return _depthStencilView;
        }

        const ComPtr<ID3D11RenderTargetView> Texture1dArray::getRenderTargetView() const {
            return _renderTargetView;
        }

        const ComPtr<ID3D11ShaderResourceView> Texture1dArray::getShaderResourceView() const {
            return _shaderResourceView;
        }

        const TextureDesc& Texture1dArray::getDescription() const {
            return _desc;
        }
    }
}