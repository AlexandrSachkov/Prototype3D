#include "GPUArray.h"

namespace p3d {
	GPUArray::GPUArray(unsigned int length) 
		: _length(length) {}

	GPUArray::GPUArray(const GPUArray& other) {
		_length = other._length;
	}

	GPUArray& GPUArray::operator=(const GPUArray& other) {
		_length = other._length;

		return *this;
	}

	GPUArray::~GPUArray() {

	}

	unsigned int GPUArray::getLength() {
		return _length;
	}
}