#include "d3d11_RenderingDevice.h"
#include "../../assert.h"
#include "d3d11_Utility.h"

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
			
			P3D_ASSERT_R(Utility::createSwapChain(
				windowHandle,
				screenRefreshRate,
				screenDim,
				msaaLevel,
				maxMsaaQualityLvl - 1,
				numBackBuffers,
				fullscreen,
				_device,
				_swapChain
			), "Failed to create swap chain");

			return true;
		}

		void RenderingDevice::release() {
			if (_swapChain) Utility::setFullScreen(_swapChain, false);

			_swapChain		= nullptr;
			_deviceContext	= nullptr;
			_device = nullptr;
		}
	}
}