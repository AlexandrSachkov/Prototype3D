#pragma once

#include "../../Constants.h"
#include <dxgiformat.h>

namespace p3d {
    namespace dx {
        DXGI_FORMAT convertVectorFormat(P3D_VECTOR_FORMAT format);
        DXGI_FORMAT convertBlockCompressionFormat(P3D_BLOCK_COMPRESSION_FORMAT format);
        bool convertFormat(Format format, DXGI_FORMAT& dxgiFormat);
    }
}