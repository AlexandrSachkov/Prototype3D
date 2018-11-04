#pragma once

#include "Texture1dArrayI.h"
#include "Texture2dArrayI.h"
#include "Texture3dI.h"
#include "VertexShaderI.h"
#include "PixelShaderI.h"

namespace p3d {

	template<class T>
	class RenderingDevice {
	public:
		bool createTexture1dArray(const Texture1dArrayDesc& desc, p3d::Texture1dArrayI*& tex) {
			return static_cast<T*>(this)->createTexture1dArray(desc, tex);
		}

		bool createTexture2dArray(const Texture2dArrayDesc& desc, p3d::Texture2dArrayI*& tex) {
			return static_cast<T*>(this)->createTexture2dArray(desc, tex);
		}

		bool createTexture3d(const Texture3dDesc& desc, p3d::Texture3dI*& tex) {
			return static_cast<T*>(this)->createTexture3d(desc, tex);
		}

		bool createVertexShader(const VertexShaderDesc& desc, p3d::VertexShaderI*& vs) {
			return static_cast<T*>(this)->createVertexShader(desc, vs);
		}

		bool createPixelShader(const PixelShaderDesc& desc, p3d::PixelShaderI*& ps) {
			return static_cast<T*>(this)->createPixelShader(desc, ps);
		}
	};
}



