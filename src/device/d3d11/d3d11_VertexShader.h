#pragma once

#include "../dx/dx_ComPtr.h"
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
            VertexShader(VertexShader&&);
            VertexShader& operator=(VertexShader&&);

            const VertexShaderDesc& getDescription() const;
            const ComPtr<ID3D11VertexShader> getShader() const;
            const ComPtr<ID3DBlob> getShaderBlob() const;
            const ComPtr<ID3D11InputLayout> getInputLayout() const;

        private:
            VertexShader(const VertexShader&) = delete;
            VertexShader& operator=(const VertexShader&) = delete;

            ComPtr<ID3D11VertexShader> _shader;
            ComPtr<ID3DBlob> _shaderBlob;
            ComPtr<ID3D11InputLayout> _inputLayout;
            VertexShaderDesc _desc;
        };
    }
}
