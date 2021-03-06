#include "d3d11_VertexShader.h"

namespace p3d {
    namespace d3d11 {
        VertexShader::VertexShader(
            ComPtr<ID3D11VertexShader> shader,
            ComPtr<ID3DBlob> shaderBlob,
            ComPtr<ID3D11InputLayout> inputLayout,
            const VertexShaderDesc& desc
        ) : _shader(shader), _shaderBlob(shaderBlob), _inputLayout(inputLayout), _desc(desc) {}

        VertexShader::VertexShader(VertexShader&& other) {
            _shader = other._shader;
            _shaderBlob = other._shaderBlob;
            _desc = other._desc;

            other._shader = nullptr;
            other._shaderBlob = nullptr;
            other._desc = {};
        }

        VertexShader& VertexShader::operator=(VertexShader&& other) {
            _shader = other._shader;
            _shaderBlob = other._shaderBlob;
            _desc = other._desc;

            other._shader = nullptr;
            other._shaderBlob = nullptr;
            other._desc = {};

            return *this;
        }

        const VertexShaderDesc& VertexShader::getDescription() const {
            return _desc;
        }

        const ComPtr<ID3D11VertexShader> VertexShader::getShader() const {
            return _shader;
        }

        const ComPtr<ID3DBlob> VertexShader::getShaderBlob() const {
            return _shaderBlob;
        }

        const ComPtr<ID3D11InputLayout> VertexShader::getInputLayout() const {
            return _inputLayout;
        }
    }
}