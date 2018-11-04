#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
	class Texture1dArrayI {
	public:
		virtual ~Texture1dArrayI() {};
		virtual Texture1dArrayDesc getDescription() = 0;
	};
}
