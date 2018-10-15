#pragma once

#include "../math/Vec2.h"

namespace p3d {

	class GPUArray {
	public:
		GPUArray(unsigned int length);
		GPUArray(const GPUArray& other);
		GPUArray& operator=(const GPUArray& other);

		virtual ~GPUArray();

		unsigned int getLength();

	private:
		GPUArray(GPUArray&&) = delete;
		GPUArray& operator=(GPUArray&&) = delete;

		unsigned int _length;
	};
}
