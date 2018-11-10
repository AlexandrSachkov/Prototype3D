#include "d3d11_Texture2dArray.h"

namespace p3d {
    namespace d3d11 {
        Texture2dArray::Texture2dArray() {}

        Texture2dArray::Texture2dArray(
            ComPtr<ID3D11Texture2D> texArr,
            ComPtr<ID3D11DepthStencilView> depthStencilView,
            ComPtr<ID3D11RenderTargetView> renderTargetView,
            ComPtr<ID3D11ShaderResourceView> shaderResourceView,
            ComPtr<ID3D11UnorderedAccessView> unorderedAccessView,
            const Texture2dArrayDesc& desc
        ) :
            _texArr(texArr),
            _depthStencilView(depthStencilView),
            _renderTargetView(renderTargetView),
            _shaderResourceView(shaderResourceView),
            _unorderedAccessView(unorderedAccessView),
            _desc(desc) {
        }

        const ComPtr<ID3D11Texture2D> Texture2dArray::Texture2dArray::getTexture() const {
            return _texArr;
        }

        const ComPtr<ID3D11DepthStencilView> Texture2dArray::getDepthStencilView() const {
            return _depthStencilView;
        }

        const ComPtr<ID3D11RenderTargetView> Texture2dArray::getRenderTargetView() const {
            return _renderTargetView;
        }

        const ComPtr<ID3D11ShaderResourceView> Texture2dArray::getShaderResourceView() const {
            return _shaderResourceView;
        }

        const ComPtr<ID3D11UnorderedAccessView> Texture2dArray::getUnorderedAccessView() const {
            return _unorderedAccessView;
        }

        const Texture2dArrayDesc& Texture2dArray::getDescription() const {
            return _desc;
        }
    }
}