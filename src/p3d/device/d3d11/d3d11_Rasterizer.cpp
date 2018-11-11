#include "d3d11_Rasterizer.h"

namespace p3d {
    namespace d3d11 {
        Rasterizer::Rasterizer(
            ComPtr<ID3D11RasterizerState> rasterizer,
            const RasterizerDesc& desc
        ) : _rasterizer(rasterizer), _desc(desc){}

        Rasterizer::Rasterizer(Rasterizer&& other) {
            _rasterizer = other._rasterizer;
            _desc = other._desc;

            other._rasterizer = nullptr;
            other._desc = {};
        }

        Rasterizer& Rasterizer::operator=(Rasterizer&& other) {
            _rasterizer = other._rasterizer;
            _desc = other._desc;

            other._rasterizer = nullptr;
            other._desc = {};

            return *this;
        }

        const ComPtr<ID3D11RasterizerState> Rasterizer::getRasterizer() const {
            return _rasterizer;
        }

        const RasterizerDesc& Rasterizer::getDescription() const {
            return _desc;
        }
    }
}