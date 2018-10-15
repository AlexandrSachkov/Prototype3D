#pragma once

#include "../RenderingDevice.h"
#include "../GPUMemoryManager.h"
#include "../../math/Vec2.h"

#include "../dx/ComPtr.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

namespace p3d {
	class Texture1dArray;
	class Texture2dArray;
}

namespace p3d {
	namespace d3d11 {

		class RenderingDevice :
			public p3d::RenderingDevice<RenderingDevice>,
			public p3d::GPUMemoryManager<RenderingDevice>{

		public:
			static RenderingDevice& instance() {
				static RenderingDevice instance;
				return instance;
			}

			bool initialize(
				HWND windowHandle,
				unsigned int screenRefreshRate,
				p3d::Vec2_uint screenDim,
				unsigned int msaaLevel,
				unsigned int numBackBuffers,
				bool fullscreen
			);
			void release();

			bool createTexture1dArray(unsigned int length, p3d::Texture1dArray*& tex);
			bool createTexture2dArray(Vec2_uint texSize, unsigned int length, p3d::Texture2dArray*& tex);
			

		protected:
			RenderingDevice();
			~RenderingDevice();

			RenderingDevice(RenderingDevice const&) = delete;            
			RenderingDevice(RenderingDevice&&) = delete;                  
			RenderingDevice& operator=(RenderingDevice const&) = delete;  
			RenderingDevice& operator=(RenderingDevice &&) = delete;      

			ComPtr<ID3D11Device> _device = nullptr;
			ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
			ComPtr<IDXGISwapChain> _swapChain = nullptr;
			ComPtr<ID3D11RenderTargetView> _backBuffRenderTargetView = nullptr;

			ComPtr<ID3D11Texture2D> _depthStencilBuff = nullptr;
			ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
		};
	}
}