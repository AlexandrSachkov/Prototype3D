#pragma once

#include "../ResourceDescriptions.h"

namespace p3d {
	class Texture2dArrayI {
	public:
		virtual ~Texture2dArrayI() {};
		virtual const Texture2dArrayDesc& getDescription() const = 0;
	};
}
