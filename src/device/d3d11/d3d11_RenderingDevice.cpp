#include "d3d11_RenderingDevice.h"
#include "../../assert.h"
#include "d3d11_Utility.h"

#include "d3d11_Texture1dArray.h"
#include "d3d11_Texture2dArray.h"

#include <Windows.h>
#include "d3d11.h"

namespace p3d {
	namespace d3d11 {

		RenderingDevice::RenderingDevice() {
			
		}

		RenderingDevice::~RenderingDevice() {
			release();
		}

		bool RenderingDevice::initialize(
			HWND windowHandle,
			unsigned int screenRefreshRate,
			p3d::Vec2_uint screenDim,
			unsigned int msaaLevel,
			unsigned int numBackBuffers,
			bool fullscreen
		) {
			P3D_ASSERT_R(Utility::createDevice(_device, _deviceContext), "d3d11 device failed to initialize");

			if (msaaLevel < 1) msaaLevel = 1; //anything less than 1 is considered off. So we set the sampling count to 1

			unsigned int maxMsaaQualityLvl = 0;
			if (msaaLevel > 1) {
				P3D_ASSERT_R(Utility::getMSAAQualityLevel(_device, msaaLevel, maxMsaaQualityLvl), "Failed to retrieve MSAA quality level");
			}

			unsigned int msaaQualityLevel = maxMsaaQualityLvl - 1;
			
			P3D_ASSERT_R(Utility::createSwapChain(
				windowHandle,
				screenRefreshRate,
				screenDim,
				msaaLevel,
				msaaQualityLevel,
				numBackBuffers,
				fullscreen,
				_device,
				_swapChain
			), "Failed to create swap chain");

			P3D_ASSERT_R(Utility::createBackBufferRenderTargetView(
				_device,
				_swapChain,
				_backBuffRenderTargetView
			), "Failed to create back buffer render target view");

			P3D_ASSERT_R(Utility::createDepthStencilView(
				_device,
				screenDim,
				msaaLevel,
				msaaQualityLevel,
				_depthStencilBuff,
				_depthStencilView
			), "Failed to create depth stencil view");

			return true;
		}

		void RenderingDevice::release() {
			if (_swapChain) Utility::setFullScreen(_swapChain, false);

			_depthStencilBuff = nullptr;
			_depthStencilView = nullptr;
			_backBuffRenderTargetView = nullptr;
			_swapChain		= nullptr;
			_deviceContext	= nullptr;
			_device			= nullptr;
		}

		bool RenderingDevice::createTexture1dArray(unsigned int length, p3d::Texture1dArray*& tex) {
			//TODO: create GPU texture array
			tex = new Texture1dArray(ComPtr<ID3D11Texture1D>(nullptr), length);
			return true;
		}

		bool RenderingDevice::createTexture2dArray(Vec2_uint texSize, unsigned int length, p3d::Texture2dArray*& tex) {
			//TODO: create GPU texture array
			tex = new Texture2dArray(ComPtr<ID3D11Texture2D>(nullptr), texSize, length);
			return true;
		}
	}
}