#pragma once

#include "../RenderingDevice.h"
#include "../../math/Vec2.h"
#include "../../ResourceDescriptions.h"
#include "../Texture1dArrayI.h"
#include "../Texture2dArrayI.h"
#include "../Texture3dI.h"
#include "../VertexShaderI.h"
#include "../PixelShaderI.h"
#include "../dx/ComPtr.h"

#include <d3d11.h>

#include <memory>

namespace p3d {
	namespace d3d11 {
		class RenderingDevice : public p3d::RenderingDevice<p3d::d3d11::RenderingDevice>{
		public:
			RenderingDevice();
			~RenderingDevice();

			bool initialize(
				HWND windowHandle,
				unsigned int screenRefreshRate,
				p3d::Vec2_uint screenDim,
				unsigned int msaaLevel,
				unsigned int numBackBuffers,
				bool fullscreen
			);
			void release();

			bool createTexture1dArray(const Texture1dArrayDesc& desc, p3d::Texture1dArrayI*& tex);
			bool createTexture2dArray(const Texture2dArrayDesc& desc, p3d::Texture2dArrayI*& tex);
			bool createTexture3d(const Texture3dDesc& desc, p3d::Texture3dI*& tex);
			
			bool createVertexShader(const VertexShaderDesc& desc, p3d::VertexShaderI*& vs);
			bool createPixelShader(const PixelShaderDesc& desc, p3d::PixelShaderI*& ps);

		private:
			RenderingDevice(RenderingDevice const&) = delete;            
			RenderingDevice(RenderingDevice&&) = delete;                  
			RenderingDevice& operator=(RenderingDevice const&) = delete;  
			RenderingDevice& operator=(RenderingDevice &&) = delete;     

			bool createInputLayout(
				const std::vector<VertexShaderDesc::InputElementDesc>& inputDesc,
				ComPtr<ID3DBlob> vsBlob,
				ComPtr<ID3D11InputLayout>& inputLayout
			);

			ComPtr<ID3D11Device> _device = nullptr;
			ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
			ComPtr<IDXGISwapChain> _swapChain = nullptr;
			ComPtr<ID3D11RenderTargetView> _backBuffRenderTargetView = nullptr;

			ComPtr<ID3D11Texture2D> _depthStencilBuff = nullptr;
			ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
		};
	}
}