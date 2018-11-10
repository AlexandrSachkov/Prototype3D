#include "d3d11_Texture3d.h"

namespace p3d {
    namespace d3d11 {
        Texture3d::Texture3d(
            ComPtr<ID3D11Texture3D> tex,
            const Texture3dDesc& desc
        ) : _tex(tex), _desc(desc) {}

        Texture3d::Texture3d(Texture3d&& other) {
            _tex = other._tex;
            _desc = other._desc;

            other._tex = nullptr;
            other._desc = {};
        }

        Texture3d& Texture3d::operator=(Texture3d&& other) {
            _tex = other._tex;
            _desc = other._desc;

            other._tex = nullptr;
            other._desc = {};

            return *this;
        }

        ComPtr<ID3D11Texture3D> Texture3d::getTexture() {
            return _tex;
        }

        Texture3dDesc Texture3d::getDescription() {
            return _desc;
        }
    }
}