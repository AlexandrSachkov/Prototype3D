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
            PixelShader(PixelShader&&);
            PixelShader& operator=(PixelShader&&);

            const PixelShaderDesc& getDescription() const;
            const ComPtr<ID3D11PixelShader> getShader() const;
            const ComPtr<ID3DBlob> getShaderBlob() const;

        private:
            PixelShader(const PixelShader&) = delete;
            PixelShader& operator=(const PixelShader&) = delete;

            ComPtr<ID3D11PixelShader> _shader;
            ComPtr<ID3DBlob> _shaderBlob;
            PixelShaderDesc _desc;
        };
    }
}
