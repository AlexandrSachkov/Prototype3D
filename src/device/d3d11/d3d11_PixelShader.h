#pragma once

#include "../dx/ComPtr.h"
#include "../PixelShaderI.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {
        class PixelShader : public PixelShaderI {
        public:
            PixelShader(
                ComPtr<ID3D11PixelShader> shader,
                ComPtr<ID3DBlob> shaderBlob,
                const PixelShaderDesc& desc);
            PixelShader(const PixelShader& other);
            ~PixelShader();

            PixelShader& operator=(const PixelShader& other);
            PixelShaderDesc getDescription();

        private:
            PixelShader(PixelShader&&) = delete;
            PixelShader& operator=(PixelShader&&) = delete;

            ComPtr<ID3D11PixelShader> _shader;
            ComPtr<ID3DBlob> _shaderBlob;
            PixelShaderDesc _desc;
        };
    }
}
