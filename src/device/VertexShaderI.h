#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
	class VertexShaderI {
	public:
		virtual ~VertexShaderI() {}
		virtual VertexShaderDesc getDescription() = 0;
	};
}