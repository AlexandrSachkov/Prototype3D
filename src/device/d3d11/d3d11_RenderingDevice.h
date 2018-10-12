#pragma once

#include "../RenderingDevice.h"
#include "../GPUMemoryManager.h"
#include "../../math/Vec2.h"

#include "../dx/ComPtr.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

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

		protected:
			RenderingDevice();
			~RenderingDevice();

			RenderingDevice(RenderingDevice const&) = delete;             // Copy construct
			RenderingDevice(RenderingDevice&&) = delete;                  // Move construct
			RenderingDevice& operator=(RenderingDevice const&) = delete;  // Copy assign
			RenderingDevice& operator=(RenderingDevice &&) = delete;      // Move assign

			ComPtr<ID3D11Device> _device = nullptr;
			ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
			ComPtr<IDXGISwapChain> _swapChain = nullptr;
		};
	}
}