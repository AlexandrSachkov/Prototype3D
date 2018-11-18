#pragma once

#include "../dx/dx_ComPtr.h"
#include "../BufferI.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {
        class Buffer : public BufferI {
        public:
            Buffer(
                ComPtr<ID3D11Buffer> buffer,
                const BufferDesc& desc
            );
            Buffer(Buffer&&);
            Buffer& operator=(Buffer&&);

            const ComPtr<ID3D11Buffer> getBuffer() const;
            const BufferDesc& getDescription() const;

        private:
            Buffer(const Buffer&) = delete;
            Buffer& operator=(const Buffer&) = delete;

            ComPtr<ID3D11Buffer> _buffer;
            BufferDesc _desc;
        };
    }
}