#include "d3d11_Texture2dArray.h"

namespace p3d {
	namespace d3d11 {
		Texture2dArray::Texture2dArray(
			ComPtr<ID3D11Texture2D> texArr,
			const Texture2dArrayDesc& desc
		) : _texArr(texArr), _desc(desc) {}

		Texture2dArray::Texture2dArray(const Texture2dArray& other) {
			_texArr = other._texArr;
			_desc = other._desc;
		}

		Texture2dArray& Texture2dArray::operator=(const Texture2dArray& other) {
			_texArr = other._texArr;
			_desc = other._desc;
			return *this;
		}

		ComPtr<ID3D11Texture2D> Texture2dArray::Texture2dArray::getTexture() {
			return _texArr;
		}

		Texture2dArrayDesc Texture2dArray::getDescription() {
			return _desc;
		}
	}
}