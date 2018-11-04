#pragma once

#include "../Texture2dArrayI.h"
#include "../dx/ComPtr.h"

#include <d3d11.h>

namespace p3d {
	namespace d3d11 {

		class Texture2dArray : public p3d::Texture2dArrayI {
		public:
			Texture2dArray(
				ComPtr<ID3D11Texture2D> texArr, 
				const Texture2dArrayDesc& desc
			);
			Texture2dArray(const Texture2dArray& other);
			Texture2dArray& operator=(const Texture2dArray& other);

			ComPtr<ID3D11Texture2D> getTexture();
			Texture2dArrayDesc getDescription();

		private:
			Texture2dArray(Texture2dArray&&) = delete;
			Texture2dArray& operator=(Texture2dArray&&) = delete;

			ComPtr<ID3D11Texture2D> _texArr;
			Texture2dArrayDesc _desc;
		};
	}
}