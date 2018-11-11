#pragma once

#include "../../Constants.h"
#include <dxgiformat.h>

namespace p3d {
    namespace dx {
        DXGI_FORMAT convertVectorFormat(P3D_VECTOR_FORMAT format);
    }
}