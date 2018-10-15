#include "d3d11_Texture2dArray.h"

namespace p3d {
	namespace d3d11 {
		Texture2dArray::Texture2dArray(
			ComPtr<ID3D11Texture2D> texArr,
			Vec2_uint texSize,
			unsigned int length
		) : _texArr(texArr), p3d::Texture2dArray(texSize, length) {}

		Texture2dArray::Texture2dArray(const Texture2dArray& other) : p3d::Texture2dArray(other){
			_texArr = other._texArr;
		}

		Texture2dArray& Texture2dArray::operator=(const Texture2dArray& other) {
			p3d::Texture2dArray::operator=(other);
			_texArr = other._texArr;
			return *this;
		}

		ComPtr<ID3D11Texture2D> Texture2dArray::Texture2dArray::getTexture() {
			return _texArr;
		}
	}
}