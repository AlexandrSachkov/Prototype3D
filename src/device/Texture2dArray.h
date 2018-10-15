#pragma once

#include "../math/Vec2.h"
#include "GPUArray.h"

namespace p3d {
	class Texture2dArray : public GPUArray {
	public:
		Texture2dArray(Vec2_uint texSize, unsigned int length);
		Texture2dArray(const Texture2dArray& other);
		Texture2dArray& operator=(const Texture2dArray& other);
		virtual ~Texture2dArray();

		Vec2_uint getTextureSize();

	private:
		Texture2dArray(Texture2dArray&&) = delete;
		Texture2dArray& operator=(Texture2dArray&&) = delete;

		Vec2_uint _texSize;
	};
}