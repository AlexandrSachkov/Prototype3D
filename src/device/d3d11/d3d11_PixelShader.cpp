#include "d3d11_PixelShader.h"

namespace p3d {
    namespace d3d11 {
        PixelShader::PixelShader(
            ComPtr<ID3D11PixelShader> shader,
            ComPtr<ID3DBlob> shaderBlob,
            const PixelShaderDesc& desc
        ) : _shader(shader), _shaderBlob(shaderBlob), _desc(desc) {
        }

        PixelShader::PixelShader(const PixelShader& other) {
            _shader = other._shader;
            _shaderBlob = other._shaderBlob;
            _desc = other._desc;
        }

        PixelShader::~PixelShader() {}

        PixelShader& PixelShader::operator=(const PixelShader& other) {
            _shader = other._shader;
            _shaderBlob = other._shaderBlob;
            _desc = other._desc;
            return *this;
        }

        PixelShaderDesc PixelShader::getDescription() {
            return _desc;
        }
    }
}