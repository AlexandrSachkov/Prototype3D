#pragma once

#include "../dx/ComPtr.h"
#include "../BufferI.h"

#include <d3d11.h>

namespace p3d {
	namespace d3d11 {
		class Buffer : public BufferI {
		public:
			Buffer(
				ComPtr<ID3D11Buffer> buffer,
				unsigned int sizeBytes,
				const BufferDesc& desc);
			~Buffer();

			const ComPtr<ID3D11Buffer> getBuffer() const;
			const BufferDesc& getDescription() const;
			unsigned int getSizeBytes() const;

		private:
			ComPtr<ID3D11Buffer> _buffer;
			unsigned int _sizeBytes;
			BufferDesc _desc;
		};
	}
}
