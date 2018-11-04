#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
	class Texture2dArrayI {
	public:
		virtual ~Texture2dArrayI() {};
		virtual Texture2dArrayDesc getDescription() = 0;
	};
}
