#include "d3d11_Buffer.h"

namespace p3d {
    namespace d3d11 {
        Buffer::Buffer(
            ComPtr<ID3D11Buffer> buffer,
            const BufferDesc& desc
        ) : _buffer(buffer), _desc(desc) {
        }

        Buffer::Buffer(Buffer&& other) {
            _buffer = other._buffer;
            _desc = other._desc;

            other._buffer = nullptr;
            other._desc = {};
        }

        Buffer& Buffer::operator=(Buffer&& other) {
            _buffer = other._buffer;
            _desc = other._desc;

            other._buffer = nullptr;
            other._desc = {};

            return *this;
        }

        const ComPtr<ID3D11Buffer> Buffer::getBuffer() const {
            return _buffer;
        }

        const BufferDesc& Buffer::getDescription() const {
            return _desc;
        }
    }
}