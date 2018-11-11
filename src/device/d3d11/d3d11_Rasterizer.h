#pragma once
#include "../dx/ComPtr.h"
#include "../RasterizerI.h"

#include <d3d11.h>

namespace p3d {
    namespace d3d11 {
        class Rasterizer : public p3d::RasterizerI {
        public:
            Rasterizer(
                ComPtr<ID3D11RasterizerState> rasterizer,
                const RasterizerDesc& desc
            );
            Rasterizer(Rasterizer&&);
            Rasterizer& operator=(Rasterizer&&);

            const ComPtr<ID3D11RasterizerState> getRasterizer() const;
            const RasterizerDesc& getDescription() const;
        private:
            ComPtr<ID3D11RasterizerState> _rasterizer;
            RasterizerDesc _desc;
        };
    }
}