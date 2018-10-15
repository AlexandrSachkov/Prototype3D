#pragma once

#include "Texture1dArray.h"
#include "Texture2dArray.h"

namespace p3d {

	template<class T>
	class GPUMemoryManager {
	public:
		bool createTexture1dArray(unsigned int length, p3d::Texture1dArray*& tex) {
			return static_cast<T*>(this)->createTexture1dArray(length, tex);
		}

		bool createTexture2dArray(Vec2_uint texSize, unsigned int length, p3d::Texture2dArray*& tex) {
			return static_cast<T*>(this)->createTexture2dArray(texSize, length, tex);
		}
	};
}

