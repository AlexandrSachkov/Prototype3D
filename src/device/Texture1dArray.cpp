#include "Texture1dArray.h"

namespace p3d {
	Texture1dArray::Texture1dArray(unsigned int length) : GPUArray(length){}

	Texture1dArray::Texture1dArray(const Texture1dArray& other) : GPUArray(other){}

	Texture1dArray& Texture1dArray::operator=(const Texture1dArray& other) {
		GPUArray::operator=(other);
		return *this;
	}
}