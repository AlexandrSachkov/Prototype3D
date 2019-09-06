#include "d3d11_Mesh.h"

namespace p3d {
    namespace d3d11 {
        Mesh::Mesh(
            ComPtr<ID3D11Buffer> vertexBuff,
            ComPtr<ID3D11Buffer> indexBuff,
            ComPtr<ID3D11Buffer> texCoordBuff,
            ComPtr<ID3D11Buffer> normalBuff,
            ComPtr<ID3D11Buffer> tangentBuff,
            ComPtr<ID3D11Buffer> bitangentBuff,
            unsigned int numVertices,
            unsigned int numIndexes,
            D3D11_PRIMITIVE_TOPOLOGY primitiveTopology
        ) : 
            _vertexBuff(vertexBuff), 
            _indexBuff(indexBuff), 
            _texCoordBuff(texCoordBuff),
            _normalBuff(normalBuff),
            _tangentBuff(tangentBuff),
            _bitangentBuff(bitangentBuff),
            _numVertices(numVertices),
            _numIndexes(numIndexes),
            _primitiveTopology(primitiveTopology)
        {}

        const ComPtr<ID3D11Buffer> Mesh::getVertexBuffer() const {
            return _vertexBuff;
        }

        const ComPtr<ID3D11Buffer> Mesh::getIndexBuffer() const {
            return _indexBuff;
        }

        const ComPtr<ID3D11Buffer> Mesh::getTextCoordBuffer() const {
            return _texCoordBuff;
        }

        const ComPtr<ID3D11Buffer> Mesh::getNormalBuffer() const {
            return _normalBuff;
        }

        const ComPtr<ID3D11Buffer> Mesh::getTangentBuffer() const {
            return _tangentBuff;
        }

        const ComPtr<ID3D11Buffer> Mesh::getBitangentBuffer() const {
            return _bitangentBuff;
        }

        unsigned int Mesh::getNumVertices() const {
            return _numVertices;
        }

        unsigned int Mesh::getNumIndexes() const {
            return _numIndexes;
        }

        D3D11_PRIMITIVE_TOPOLOGY Mesh::getPrimitiveTopology() const {
            return _primitiveTopology;
        }
    }
}