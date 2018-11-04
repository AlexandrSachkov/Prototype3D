#pragma once

#include "Constants.h"

#include <vector>
#include <string>

namespace p3d {
	struct Texture1dArrayDesc {

	};

	struct Texture2dArrayDesc {

	};

	struct Texture3dDesc {

	};

	struct VertexShaderDesc {
		struct InputElementDesc {
			std::string elementName;
			P3D_FORMAT dataFormat;
			unsigned int dataSizeBytes;
			unsigned int inputSlot;
		};

		std::vector<InputElementDesc> inputDesc;
		std::string hlslSource;
		std::string shaderEntryPoint;
	};

	struct PixelShaderDesc {
		std::string hlslSource;
		std::string shaderEntryPoint;
	};
}