#pragma once
#include "../../Constants.h"
#include <d3d11.h>

namespace p3d {
	namespace d3d11 {
		bool convertBindFlag(P3D_BIND_FLAG flag, D3D11_BIND_FLAG& d3d11Flag);

		struct UsageDesc {
			D3D11_USAGE usage;
			D3D11_CPU_ACCESS_FLAG cpuAccessFlag;
		};

		bool convertUsageFlag(P3D_USAGE flag, UsageDesc& usageDesc);
	}
}