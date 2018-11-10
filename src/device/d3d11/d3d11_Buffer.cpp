#include "d3d11_Buffer.h"

namespace p3d {
	namespace d3d11 {
		Buffer::Buffer( 
			ComPtr<ID3D11Buffer> buffer, 
			unsigned int sizeBytes,
			const BufferDesc& desc
		) : _buffer(buffer), _sizeBytes(sizeBytes), _desc(desc) {}

		Buffer::~Buffer() {}

		const ComPtr<ID3D11Buffer> Buffer::getBuffer() const {
			return _buffer;
		}

		const BufferDesc& Buffer::getDescription() const {
			return _desc;
		}

		unsigned int Buffer::getSizeBytes() const {
			return _sizeBytes;
		}
	}
}