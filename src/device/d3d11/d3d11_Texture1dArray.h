#pragma once
#include "../dx/ComPtr.h"
#include "../Texture1dArrayI.h"
#include <d3d11.h>

namespace p3d {
    namespace d3d11 {

        class Texture1dArray : public p3d::Texture1dArrayI {
        public:
            Texture1dArray(ComPtr<ID3D11Texture1D> texArr, const Texture1dArrayDesc& desc);
            Texture1dArray(const Texture1dArray& other);
            Texture1dArray& operator=(const Texture1dArray& other);

            ComPtr<ID3D11Texture1D> getTexture();
            Texture1dArrayDesc getDescription();

        private:
            Texture1dArray(Texture1dArray&&) = delete;
            Texture1dArray& operator=(Texture1dArray&&) = delete;

            ComPtr<ID3D11Texture1D> _texArr;
            Texture1dArrayDesc _desc;
        };
    }
}