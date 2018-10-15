#include "Texture2dArray.h"

namespace p3d {
	Texture2dArray::Texture2dArray(Vec2_uint texSize, unsigned int length)
		: _texSize(texSize), GPUArray(length) {}

	Texture2dArray::Texture2dArray(const Texture2dArray& other) : GPUArray(other) {
		_texSize = other._texSize;
	}

	Texture2dArray& Texture2dArray::operator=(const Texture2dArray& other) {
		GPUArray::operator=(other);
		_texSize = other._texSize;

		return *this;
	}

	Texture2dArray::~Texture2dArray() {}

	Vec2_uint Texture2dArray::getTextureSize() {
		return _texSize;
	}
}