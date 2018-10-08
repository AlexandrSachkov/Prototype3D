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

		}

		bool RenderingDevice::initialize() {
			P3D_ASSERT_R(true, "d3d11 device failed to initialize");
			return true;
		}

		void RenderingDevice::release() {

		}
	}
}