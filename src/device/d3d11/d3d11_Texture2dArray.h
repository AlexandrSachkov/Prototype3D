#pragma once

#include "../Texture2dArray.h"
#include "../dx/ComPtr.h"

#include <d3d11.h>

namespace p3d {
	namespace d3d11 {

		class Texture2dArray : public p3d::Texture2dArray{
		public:
			Texture2dArray(
				ComPtr<ID3D11Texture2D> texArr, 
				Vec2_uint texSize, 
				unsigned int length
			);
			Texture2dArray(const Texture2dArray& other);
			Texture2dArray& operator=(const Texture2dArray& other);

			ComPtr<ID3D11Texture2D> getTexture();

		private:
			Texture2dArray(Texture2dArray&&) = delete;
			Texture2dArray& operator=(Texture2dArray&&) = delete;

			ComPtr<ID3D11Texture2D> _texArr;
		};
	}
}