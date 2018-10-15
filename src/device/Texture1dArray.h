#pragma once

#include "GPUArray.h"

namespace p3d {
	class Texture1dArray : public GPUArray {
	public:
		Texture1dArray(unsigned int length);
		Texture1dArray(const Texture1dArray& other);
		Texture1dArray& operator=(const Texture1dArray& other);

	private:
		Texture1dArray(Texture1dArray&&) = delete;
		Texture1dArray& operator=(Texture1dArray&&) = delete;
	};
}
