#include "d3d11_Texture1dArray.h"

namespace p3d {
	namespace d3d11 {
		Texture1dArray::Texture1dArray(ComPtr<ID3D11Texture1D> texArr, unsigned int length) 
			: _texArr(texArr), p3d::Texture1dArray(length){}

		Texture1dArray::Texture1dArray(const Texture1dArray& other) : p3d::Texture1dArray(other) {
			_texArr = other._texArr;
		}

		Texture1dArray& Texture1dArray::operator=(const Texture1dArray& other) {
			p3d::Texture1dArray::operator=(other);
			_texArr = other._texArr;
			return *this;
		}

		ComPtr<ID3D11Texture1D> Texture1dArray::Texture1dArray::getTexture() {
			return _texArr;
		}
	}
}