#pragma once

#include "../Texture3dI.h"
#include "../dx/dx_ComPtr.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {

        class Texture3d : public p3d::Texture3dI {
        public:
            Texture3d(
                ComPtr<ID3D11Texture3D> tex,
                const Texture3dDesc& desc
            );
            Texture3d(Texture3d&&);
            Texture3d& operator=(Texture3d&&);

            ComPtr<ID3D11Texture3D> getTexture();
            Texture3dDesc getDescription();

        private:
            Texture3d(const Texture3d&) = delete;
            Texture3d& operator=(const Texture3d&) = delete;

            ComPtr<ID3D11Texture3D> _tex;
            Texture3dDesc _desc;
        };
    }
}