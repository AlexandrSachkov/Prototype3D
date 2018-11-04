#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
	class PixelShaderI {
	public:
		virtual ~PixelShaderI() {}
		virtual PixelShaderDesc getDescription() = 0;
	};
}
