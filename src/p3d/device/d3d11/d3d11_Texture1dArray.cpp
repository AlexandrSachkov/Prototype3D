#include "d3d11_Texture1dArray.h"

namespace p3d {
    namespace d3d11 {
        Texture1dArray::Texture1dArray(ComPtr<ID3D11Texture1D> texArr, const Texture1dArrayDesc& desc)
            : _texArr(texArr), _desc(desc) {}

        Texture1dArray::Texture1dArray(Texture1dArray&& other) {
            _texArr = other._texArr;
            _desc = other._desc;

            other._texArr = nullptr;
            other._desc = {};
        }

        Texture1dArray& Texture1dArray::operator=(Texture1dArray&& other) {
            _texArr = other._texArr;
            _desc = other._desc;

            other._texArr = nullptr;
            other._desc = {};

            return *this;
        }

        ComPtr<ID3D11Texture1D> Texture1dArray::Texture1dArray::getTexture() {
            return _texArr;
        }

        Texture1dArrayDesc Texture1dArray::getDescription() {
            return _desc;
        }
    }
}