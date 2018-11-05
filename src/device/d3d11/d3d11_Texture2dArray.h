#pragma once

#include "../Texture2dArrayI.h"
#include "../dx/ComPtr.h"

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
				const Texture2dArrayDesc& desc
			);

			const ComPtr<ID3D11Texture2D> getTexture() const;
			const ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
			const ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;
			const ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
			const ComPtr<ID3D11UnorderedAccessView> getUnorderedAccessView() const;
			const Texture2dArrayDesc& getDescription() const;

		private:
			ComPtr<ID3D11Texture2D> _texArr;
			ComPtr<ID3D11DepthStencilView> _depthStencilView;
			ComPtr<ID3D11RenderTargetView> _renderTargetView;
			ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
			ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView;
			Texture2dArrayDesc _desc;
		};
	}
}