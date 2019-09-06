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
                unsigned int numVertices,
                unsigned int numIndexes,
                D3D11_PRIMITIVE_TOPOLOGY primitiveTopology
            );

            const ComPtr<ID3D11Buffer> getVertexBuffer() const;
            const ComPtr<ID3D11Buffer> getIndexBuffer() const;
            const ComPtr<ID3D11Buffer> getTextCoordBuffer() const;
            const ComPtr<ID3D11Buffer> getNormalBuffer() const;
            const ComPtr<ID3D11Buffer> getTangentBuffer() const;
            const ComPtr<ID3D11Buffer> getBitangentBuffer() const;
            unsigned int getNumVertices() const;
            unsigned int getNumIndexes() const;
            D3D11_PRIMITIVE_TOPOLOGY getPrimitiveTopology() const;
        private:
            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;

            ComPtr<ID3D11Buffer> _vertexBuff;
            ComPtr<ID3D11Buffer> _indexBuff;
            ComPtr<ID3D11Buffer> _texCoordBuff;
            ComPtr<ID3D11Buffer> _normalBuff;
            ComPtr<ID3D11Buffer> _tangentBuff;
            ComPtr<ID3D11Buffer> _bitangentBuff;
            unsigned int _numVertices = 0;
            unsigned int _numIndexes = 0;
            D3D11_PRIMITIVE_TOPOLOGY _primitiveTopology;
        };
    }
}