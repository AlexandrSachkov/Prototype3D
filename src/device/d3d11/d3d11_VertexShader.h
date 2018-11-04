#pragma once

#include "../dx/ComPtr.h"
#include "../VertexShaderI.h"

#include <d3d11.h>

namespace p3d {
	namespace d3d11 {
		class VertexShader : public VertexShaderI {
		public:
			VertexShader(
				ComPtr<ID3D11VertexShader> shader, 
				ComPtr<ID3DBlob> shaderBlob,
				ComPtr<ID3D11InputLayout> inputLayout,
				const VertexShaderDesc& desc);
			VertexShader(const VertexShader& other);
			~VertexShader();

			VertexShader& operator=(const VertexShader& other);
			VertexShaderDesc getDescription();

		private:
			VertexShader(VertexShader&&) = delete;
			VertexShader& operator=(VertexShader&&) = delete;

			ComPtr<ID3D11VertexShader> _shader;
			ComPtr<ID3DBlob> _shaderBlob;
			ComPtr<ID3D11InputLayout> _inputLayout;
			VertexShaderDesc _desc;
		};
	}
}
