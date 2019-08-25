#pragma once

#include "../MeshI.h"
#include "../dx/dx_ComPtr.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {
        class Mesh : public MeshI {
        public:
            Mesh(
                ComPtr<ID3D11Buffer> vertexBuff,
                ComPtr<ID3D11Buffer> indexBuff,
                ComPtr<ID3D11Buffer> texCoordBuff,
                ComPtr<ID3D11Buffer> normalBuff,
                ComPtr<ID3D11Buffer> tangentBuff,
                ComPtr<ID3D11Buffer> bitangentBuff,
                ComPtr<ID3D11Buffer> colorBuff
            );

            const ComPtr<ID3D11Buffer> getVertexBuffer() const;
            const ComPtr<ID3D11Buffer> getIndexBuffer() const;
            const ComPtr<ID3D11Buffer> getTextCoordBuffer() const;
            const ComPtr<ID3D11Buffer> getNormalBuffer() const;
            const ComPtr<ID3D11Buffer> getTangentBuffer() const;
            const ComPtr<ID3D11Buffer> getBitangentBuffer() const;
            const ComPtr<ID3D11Buffer> getColorBuffer() const;

        private:
            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;

            ComPtr<ID3D11Buffer> _vertexBuff;
            ComPtr<ID3D11Buffer> _indexBuff;
            ComPtr<ID3D11Buffer> _texCoordBuff;
            ComPtr<ID3D11Buffer> _normalBuff;
            ComPtr<ID3D11Buffer> _tangentBuff;
            ComPtr<ID3D11Buffer> _bitangentBuff;
            ComPtr<ID3D11Buffer> _colorBuff;
        };
    }
}